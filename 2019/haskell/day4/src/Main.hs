module Main where

import Data.List.Ordered

count = (length .) . filter

isValid extraCheck pwd = let chars = show pwd in
  let ordered = isSorted chars
      duplicateFinder elem = extraCheck (count (== elem) chars) 2
      duplicatesFound = any duplicateFinder chars
  in ordered && duplicatesFound

countValid = flip count [153517..630395] . isValid

main = do
  print $ countValid (>=)
  print $ countValid (==)