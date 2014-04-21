import Control.Monad
import qualified Data.ByteString.Char8 as B
import Data.List
import Data.Maybe

byteStrToInt :: B.ByteString -> Int
byteStrToInt = fst . fromJust . B.readInt

solveProblem :: B.ByteString -> Bool
solveProblem s = not $ and $ map f ["101", "010"]
  where
  f = B.null . snd . flip B.breakSubstring s . B.pack

breakUnfold :: (a -> (b, B.ByteString)) -> a -> Maybe(b, B.ByteString)
breakUnfold f x = case f x of
  (y, z)
    | B.length z == 0 -> Nothing
    | otherwise -> Just (y, B.tail z)

main :: IO()
main = do
  ss <- B.getContents
  let (l, ls) = B.break (== '\n') ss
      numTests = byteStrToInt l
      rs = Data.List.unfoldr (breakUnfold $ B.break (== '\n')) $ B.tail ls
  foldM_ f rs [1..numTests]
    where
    f (tl:tls) _ = do
      let answer = solveProblem tl
      putStrLn $ if answer then "Good" else "Bad"
      return tls
    f [] _ = return []
