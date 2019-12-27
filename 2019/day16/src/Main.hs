module Main where

digits :: String -> [Int]
digits = map (read . return)

patt :: Int -> [Int]
patt = tail . cycle . ([0, 1, 0, -1] >>=) . replicate

fft :: [Int] -> [Int]
fft l = map (\x -> ((abs . sum . zipWith (*) l) . patt) x `mod` 10) [1 .. (length l)]

part1fft :: String -> [Int]
part1fft str = take 8 $ iterate fft (digits str) !! 100

input :: String
input = "03036732577212944063491565474664"

main :: IO ()
main = do
  print $ part1fft input
  print $ part2fft input
