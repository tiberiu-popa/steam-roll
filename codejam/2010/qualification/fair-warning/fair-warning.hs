import Control.Monad
import Data.List
import Data.Tuple
import System.Environment (getArgs)
import System.IO

main :: IO ()
main = do
	args <- getArgs
	hs <- openDefaultFiles args
	hvInteractLines hs $ zipWith printCase [(1::Integer) ..] . map (solveProblem . map read . tail . words) . tail
	hMultiClose hs

solveProblem :: [Integer] -> Integer
solveProblem = solve . sort
	where
	solve xs = if r == 0 then 0 else gs - r
		where
		gs = diffgcd xs
		r = (head xs) `mod` gs
	diffgcd xs = multigcd $ zipWith (-) (tail xs) xs
	multigcd (x:[]) = x
	multigcd (x:xs) = gcd x (multigcd xs)
	multigcd _ = 1

mapLines :: ([String] -> [String]) -> String -> String
mapLines f = unlines . f . lines

interactLines :: ([String] -> [String]) -> IO ()
interactLines = interact . mapLines

hvInteractLines :: [Handle] -> ([String] -> [String]) -> IO ()
hvInteractLines hs = hInteractLines (head hs) (head $ tail hs)

hInteractLines :: Handle -> Handle -> ([String] -> [String]) -> IO ()
hInteractLines hIn hOut = hInteract hIn hOut . mapLines

printCase :: (Show t, Show s) => t -> s -> String
printCase caseId result = "Case #" ++ show caseId ++ ": " ++ show result

hInteract :: Handle -> Handle -> (String -> String) -> IO ()
hInteract hIn hOut f = do
	s <- hGetContents hIn
	hPutStr hOut (f s)

openFiles :: [FilePath] -> IO [Handle]
openFiles = mapM (uncurry openFile . swap) . zip [ ReadMode, WriteMode ]

openDefaultFiles :: [FilePath] -> IO [Handle]
openDefaultFiles = (liftM2 defaultize) (mapM return defaultHandles) . openFiles

hMultiClose :: [Handle] -> IO ()
hMultiClose = mapM_ hClosePair . zip defaultHandles
	where
	hClosePair (x, y)
		| x == y = return ()
		| otherwise = hClose y

defaultHandles :: [Handle]
defaultHandles = [ stdin, stdout ]

defaultize :: [t] -> [t] -> [t]
defaultize xs [] = xs
defaultize [] _ = []
defaultize (_:xs) (y:ys) = y:(defaultize xs ys)
