module Main where

import Data.List as L
import Data.Vector.Unboxed as V

data State = State { memory :: Vector Int
                   , inputs :: Vector Int
                   , ip :: Int 
                   , relative :: Int } deriving Show

data Mode = Position | Immediate | Relative deriving Show

data Result = Done (Maybe Int) | Ongoing State deriving Show

toMode :: Int -> Mode
toMode 0 = Position
toMode 1 = Immediate
toMode 2 = Relative

findAddress :: State -> Mode -> Int -> Int
findAddress state mode offset = let currentIp = (ip state) + offset in
  case mode of
    Position -> (memory state) ! currentIp
    Immediate -> currentIp
    Relative -> (relative state) + ((memory state) ! currentIp)

parseModes :: Int -> Int -> [Mode]
parseModes numModes instruction = case numModes of
  0 -> []
  _ -> [toMode $ instruction `mod` 10] L.++ parseModes (numModes - 1) (instruction `div` 10)

incrementInstruction :: Int -> State -> (State, [Int])
incrementInstruction args state = let instruction = (memory state) ! (ip state)
                                      modes = parseModes args (instruction `div` 100)
                                      addresses = L.map (\ (mode, offset) -> findAddress state mode offset) (L.zip modes [1..])
                                      newIp = ip state + args + 1
  in (state { ip = newIp }, addresses)

intcodeOp State { memory = m, inputs = i, ip = ip, relative = r } fn = 
    let new = fn (m ! (m ! (ip + 1))) (m ! (m ! (ip + 2))) in
    State { memory = m // [(m ! (ip + 3), new)], inputs = i, ip = ip + 4, relative = r}

increment s@State { memory = memory, ip = ip } = case memory ! ip of
    1 -> Ongoing $ intcodeOp s (+)
    2 -> Ongoing $ intcodeOp s (*)
    99 -> Done $ return $ memory ! 0
    _ -> Done Nothing

runProgram s = case increment s of
    Done p -> p
    Ongoing s -> runProgram s

main = print "yo"