import qualified Data.ByteString.Lazy.Char8 as L
import Control.Monad
import Data.Maybe

byteStrToInt :: L.ByteString -> Int
byteStrToInt = fst . fromJust . L.readInt

byteStrToFloat :: L.ByteString -> Float
byteStrToFloat = (fromIntegral :: (Int -> Float)) . byteStrToInt

solveProblem :: (Floating a, Ord a) => a -> a -> a -> a -> Bool
solveProblem likes d s c = log s + (d - 1) * log (c + 1) >= log likes

main :: IO()
main = do
  (line:ls) <- fmap L.lines L.getContents
  let numTests = byteStrToInt line
  foldM_ f ls [1..numTests]
    where
    f (tl:tls) _ = do
      let [likes, d, s, c] = map byteStrToFloat (L.split ' ' tl)
          answer = solveProblem likes d s c
      putStrLn $ if answer then "ALIVE AND KICKING" else "DEAD AND ROTTING"
      return tls
    f [] _ = return []
