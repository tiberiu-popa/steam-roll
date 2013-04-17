-- holy yelling of mother jesus christ's uncle
import Control.Monad
import qualified Data.Foldable
import qualified Data.HashMap
import qualified Data.Hashable
import qualified Data.List
import qualified Data.Maybe
import qualified Data.MultiSet
import qualified Data.Sequence
import qualified Data.Set

getLines :: FilePath -> IO [String]
getLines = liftM lines . readFile

parseFwContents :: Int -> [String] -> [([Int], [(Int,[Int])])]
parseFwContents n contents
	| n > 0 = nextResult:(parseFwContents (n - 1) nextContents)
	| otherwise = [ ]
		where
		(nextResult, nextContents) = parseKM (head contents) (tail contents)
		parseKM line rest = parseKeys m (head rest) (tail rest)
			where
			[ _, m ] = map (read :: String -> Int) (words $ line)
		parseKeys m line rest = ((keys, result), newRest)
			where
			keys = map (read :: String -> Int) (words $ line)
			(result, newRest) = parseChests keys m rest
		parseChests keys m chestLines
			| m > 0 = ((head chest, tail $ tail chest):result, newLines)
			| otherwise = ([ ], chestLines)
				where
				(result, newLines) = parseChests keys (m - 1) $ tail chestLines
				chest = map (read :: String -> Int) (words $ head chestLines)

increment :: Maybe Int -> Maybe Int
increment x = Just $ 1 + Data.Maybe.fromMaybe 0 x

parseContents :: [String] -> [([Int], [(Int,[Int])])]
parseContents contents = parseFwContents n (tail contents)
	where
	n = read (head contents) :: Int

createCountMap :: (Ord k, Data.Hashable.Hashable k) => Data.HashMap.Map k Int -> [k] -> Data.HashMap.Map k Int
createCountMap accum [] = accum
createCountMap accum (key:ks) = createCountMap (Data.HashMap.alter increment key accum) ks

updateSet :: t -> Maybe [t] -> Maybe [t]
updateSet v Nothing = Just [v]
updateSet v (Just vs) = Just $ v:vs

reduceGraph :: (Ord k, Data.Hashable.Hashable k) => [(k, t)] -> Data.HashMap.Map k [t]
reduceGraph [] = Data.HashMap.empty
reduceGraph (chest:cs) = Data.HashMap.alter (updateSet $ snd chest) (fst chest) rest
	where
	rest = reduceGraph cs

collapseGraph :: [(Int, Data.Set.Set Int)] -> Data.HashMap.Map Int [Int]
collapseGraph = Data.HashMap.map (Data.Set.toList . Data.Set.unions) . reduceGraph

unique :: Ord t => [ t ] -> [ t ]
unique xs = Data.Set.toList $ Data.Set.fromList xs

keySet :: Ord t => Data.HashMap.Map t s -> Data.Set.Set t
keySet hm = Data.Set.fromList $ Data.HashMap.keys hm

bfs :: (Ord t, Data.Hashable.Hashable t) => Data.Sequence.Seq t -> Data.Set.Set t -> Data.HashMap.Map t [t] -> Data.Set.Set t
bfs queue visited graph
	| Data.Sequence.null queue = visited
	| otherwise = bfs (qs Data.Sequence.>< Data.Sequence.fromList newEdges) newVisited graph
		where
		element = Data.Sequence.index queue 0
		qs = Data.Sequence.drop 1 queue
		newVisited = Data.Set.insert element visited
		edges = Data.Maybe.fromMaybe [] $ Data.HashMap.lookup element graph
		newEdges = filter (not . (flip Data.Set.member) newVisited) edges

areKeysEnough :: (Ord s, Num t, Ord t) => Data.HashMap.Map s t -> Data.HashMap.Map s t -> Bool
areKeysEnough inKeys outKeys = all (>= 0) $ Data.HashMap.elems diffKeys
	where
	invOutKeys = Data.HashMap.map (0 -) outKeys
	diffKeys = Data.HashMap.unionWith (+) inKeys invOutKeys

allReachable :: [Int] -> [(Int, [Int])] -> Data.Set.Set Int -> Bool
allReachable keys chests allKeys = Data.Set.isSubsetOf allKeys reachableKeys
	where
	fullGraph = map (\(x, y) -> (x, Data.Set.fromList y)) chests
	graph = collapseGraph fullGraph
	initialSeq = Data.Sequence.fromList $ unique keys
	reachableKeys = bfs initialSeq Data.Set.empty graph

findSmallestIndex :: Data.MultiSet.MultiSet Int -> [(Int, (Int, [Int]))] -> [(Int, (Int, [Int]))] -> Int
findSmallestIndex _ _ [] = undefined
findSmallestIndex keys prefix (entry:suffix)
	| Data.MultiSet.member input keys && valid = entryIndex
	| otherwise = findSmallestIndex keys (entry:prefix) suffix
	where
	newKeys = Data.MultiSet.union (Data.MultiSet.delete input keys) (Data.MultiSet.fromList edges)
	newChests = map snd $ prefix ++ suffix
	newKeyList = Data.Set.toList $ Data.MultiSet.toSet newKeys
	outKeys = createCountMap Data.HashMap.empty $ map fst newChests
	allKeys = keySet outKeys
	valid = allReachable newKeyList newChests allKeys
	(entryIndex, vertex) = entry
	(input, edges) = vertex

findPermutation :: Data.Sequence.Seq Int -> Data.MultiSet.MultiSet Int -> [(Int, (Int, [Int]))] -> Data.Sequence.Seq Int
findPermutation accum _ [ ] = accum
findPermutation accum keys chests = findPermutation (accum Data.Sequence.|> 1 + firstIndex) newKeys rc
	where
	firstIndex = findSmallestIndex keys [ ] chests
	((_, lc):_, rc) = Data.List.partition (\x -> fst x == firstIndex) chests
	newKeys = Data.MultiSet.union (Data.MultiSet.delete (fst lc) keys) (Data.MultiSet.fromList $ snd lc)

solveProblem :: ([Int], [(Int, [Int])]) -> Data.Sequence.Seq Int
solveProblem (keys, chests)
	| enoughKeys && Data.Set.isSubsetOf allKeys reachableKeys = findPermutation Data.Sequence.empty (Data.MultiSet.fromList keys) $ zip [0..] chests
	| otherwise = Data.Sequence.empty
	where
	initialKeys = createCountMap Data.HashMap.empty keys
	inKeys = createCountMap initialKeys $ concat $ map snd chests
	outKeys = createCountMap Data.HashMap.empty $ map fst chests
	enoughKeys = areKeysEnough inKeys outKeys
	fullGraph = map (\(x, y) -> (x, Data.Set.fromList y)) chests
	graph = collapseGraph fullGraph
	allKeys = keySet outKeys
	initialSeq = Data.Sequence.fromList $ unique keys
	reachableKeys = bfs initialSeq Data.Set.empty graph

prettySolver :: (Num t, Show t) => t -> ([Int], [(Int, [Int])]) -> String
prettySolver i contents
	| Data.Sequence.null solution = caseStr ++ " IMPOSSIBLE"
	| otherwise = unwords $ caseStr : (map show . Data.Foldable.toList $ solution)
	where
	solution = solveProblem contents
	caseStr = "Case #" ++ show (i + 1) ++ ":"

zmap :: (Int -> t -> s) -> [t] -> [s]
zmap f v = zmap' 0 v
	where
	zmap' _ [ ] = [ ]
	zmap' i (x:xs) = (f i x):(zmap' (i + 1) xs)

main :: IO ()
main = do
	contents <- getLines "treasure.in"
	forM_ (zmap prettySolver $ parseContents contents) putStrLn
