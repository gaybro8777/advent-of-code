module Main where

import Data.List.Ordered

count = (length .) . filter

isValid extraCheck pwd = let chars = show pwd in
  let ordered = isSorted chars in
  let duplicateFinder elem = extraCheck (count (== elem) chars) 2 in
  let duplicatesFound = any duplicateFinder chars in
  ordered && duplicatesFound

countValid = flip count [153517..630395] . isValid

main = do
  print $ countValid (>=)
  print $ countValid (==)