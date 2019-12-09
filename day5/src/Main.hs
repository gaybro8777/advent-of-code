module Main where

import Data.List as L
import Data.Int
import Data.Vector.Unboxed as V
import Debug.Trace

data State = State { memory :: Vector Int64
                   , inputs :: Vector Int64
                   , outputs :: Vector Int64
                   , ip :: Int64 } deriving Show

data Mode = Position | Immediate | Relative deriving Show

data Result = Done (Maybe Int64) | Ongoing State deriving Show

toMode :: Int64 -> Mode
toMode 0 = Position
toMode 1 = Immediate

findAddress :: State -> Mode -> Int64 -> Int64
findAddress State { memory = m, ip = ip } mode offset = 
  let currentIp = ip + offset 
  in case mode of
    Position -> m ! fromIntegral currentIp
    Immediate -> currentIp

parseModes :: Int64 -> Int64 -> [Mode]
parseModes numModes instruction = case numModes of
  0 -> []
  _ -> [toMode $ instruction `mod` 10] L.++ parseModes (numModes - 1) (instruction `div` 10)

incrementInstruction :: Int64 -> State -> (State, Vector Int64)
incrementInstruction args s@State { memory = m, ip = ip } =
  let instruction = m ! fromIntegral ip
      modes = parseModes args (instruction `div` 100)
      addresses = L.zipWith (findAddress s) modes [1..]
      newIp = ip + args + 1
  in (s { ip = newIp }, fromList addresses)

vecSet :: Int64 -> Int64 -> Vector Int64 -> Vector Int64
vecSet ind value vec = 
  let extra = V.replicate (max 0 1 + (fromIntegral ind - V.length vec)) 0 
  in (vec V.++ extra) // [(fromIntegral ind, value)]

add :: State -> State
add s@State { memory = m } =
  let (newState, params) = incrementInstruction 3 s
      load ind = m ! fromIntegral (params ! ind)
  in newState { memory = vecSet (fromIntegral $ params ! 2) (load 0 + load 1) m }

mul :: State -> State
mul s@State { memory = m } =
  let (newState, params) = incrementInstruction 3 s
      load ind = m ! fromIntegral (params ! ind)
  in newState { memory = vecSet (fromIntegral $ params ! 2) (load 0 * load 1) m }

input :: State -> State
input s@State { memory = m, inputs = i } =
  let (newState, params) = incrementInstruction 1 s
  in newState { memory = vecSet (fromIntegral $ params ! 0) (V.head i) m
              , inputs = V.tail i }

jt :: State -> State
jt s@State { memory = m } =
  let (newState, params) = incrementInstruction 2 s
      load ind = m ! fromIntegral (params ! ind)
  in newState { ip = if load 0 /= 0 then load 1 else ip newState }

jf :: State -> State
jf s@State { memory = m } =
  let (newState, params) = incrementInstruction 2 s
      load ind = m ! fromIntegral (params ! ind)
  in newState { ip = if load 0 == 0 then load 1 else ip newState }

lt :: State -> State
lt s@State { memory = m } =
  let (newState, params) = incrementInstruction 3 s
      load ind = m ! fromIntegral (params ! ind)
  in newState { memory = vecSet (fromIntegral $ params ! 2) (if load 0 < load 1 then 1 else 0) m }

eq :: State -> State
eq s@State { memory = m } =
  let (newState, params) = incrementInstruction 3 s
      load ind = m ! fromIntegral (params ! ind)
  in newState { memory = vecSet (fromIntegral $ params ! 2) (if load 0 == load 1 then 1 else 0) m }

ret :: State -> State
ret s@State { memory = m, outputs = o } =
  let (newState, params) = incrementInstruction 1 s
  in newState { outputs = snoc o (m ! fromIntegral (params ! 0)) }

increment :: State -> Result
increment s@State { memory = m, ip = ip, outputs = o } = 
  case (m ! fromIntegral ip) `mod` 100 of
    1 -> Ongoing $ add s
    2 -> Ongoing $ mul s
    3 -> Ongoing $ input s
    4 -> Ongoing $ ret s
    5 -> Ongoing $ jt s
    6 -> Ongoing $ jf s
    7 -> Ongoing $ lt s
    8 -> Ongoing $ eq s
    99 -> Done $ Just $ V.last o
    _ -> Done Nothing

runProgram :: State -> Maybe Int64
runProgram s = case increment s of
  Done p -> p
  Ongoing s -> runProgram s

program :: Vector Int64
program = fromList [3,225,1,225,6,6,1100,1,238,225,104,0,1102,27,28,225,1,113,14,224,1001,224,-34,224,4,224,102,8,223,223,101,7,224,224,1,224,223,223,1102,52,34,224,101,-1768,224,224,4,224,1002,223,8,223,101,6,224,224,1,223,224,223,1002,187,14,224,1001,224,-126,224,4,224,102,8,223,223,101,2,224,224,1,224,223,223,1102,54,74,225,1101,75,66,225,101,20,161,224,101,-54,224,224,4,224,1002,223,8,223,1001,224,7,224,1,224,223,223,1101,6,30,225,2,88,84,224,101,-4884,224,224,4,224,1002,223,8,223,101,2,224,224,1,224,223,223,1001,214,55,224,1001,224,-89,224,4,224,102,8,223,223,1001,224,4,224,1,224,223,223,1101,34,69,225,1101,45,67,224,101,-112,224,224,4,224,102,8,223,223,1001,224,2,224,1,223,224,223,1102,9,81,225,102,81,218,224,101,-7290,224,224,4,224,1002,223,8,223,101,5,224,224,1,223,224,223,1101,84,34,225,1102,94,90,225,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,1007,677,677,224,102,2,223,223,1005,224,329,101,1,223,223,1108,226,677,224,1002,223,2,223,1005,224,344,101,1,223,223,1008,677,677,224,102,2,223,223,1005,224,359,101,1,223,223,8,226,677,224,1002,223,2,223,1006,224,374,101,1,223,223,108,226,677,224,1002,223,2,223,1006,224,389,1001,223,1,223,1107,226,677,224,102,2,223,223,1005,224,404,1001,223,1,223,7,226,677,224,1002,223,2,223,1005,224,419,101,1,223,223,1107,677,226,224,102,2,223,223,1006,224,434,1001,223,1,223,1107,226,226,224,1002,223,2,223,1006,224,449,101,1,223,223,1108,226,226,224,1002,223,2,223,1005,224,464,101,1,223,223,8,677,226,224,102,2,223,223,1005,224,479,101,1,223,223,8,226,226,224,1002,223,2,223,1006,224,494,1001,223,1,223,1007,226,677,224,1002,223,2,223,1006,224,509,1001,223,1,223,108,226,226,224,1002,223,2,223,1006,224,524,1001,223,1,223,1108,677,226,224,102,2,223,223,1006,224,539,101,1,223,223,1008,677,226,224,102,2,223,223,1006,224,554,101,1,223,223,107,226,677,224,1002,223,2,223,1006,224,569,101,1,223,223,107,677,677,224,102,2,223,223,1006,224,584,101,1,223,223,7,677,226,224,102,2,223,223,1005,224,599,101,1,223,223,1008,226,226,224,1002,223,2,223,1005,224,614,1001,223,1,223,107,226,226,224,1002,223,2,223,1005,224,629,101,1,223,223,7,226,226,224,102,2,223,223,1006,224,644,1001,223,1,223,1007,226,226,224,102,2,223,223,1006,224,659,101,1,223,223,108,677,677,224,102,2,223,223,1005,224,674,1001,223,1,223,4,223,99,226]

main = do
  let run input = runProgram State { memory = program
                                   , inputs = fromList [input]
                                   , outputs = V.empty
                                   , ip = 0 }
  print $ run 1
  print $ run 5