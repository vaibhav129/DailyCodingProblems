#include <bits/stdc++.h>
using namespace std;

/*
PROBLEM STATEMENT:
Given a positive integer N, find the smallest number of steps it will take to reach 1.

PERMITTED STEPS:
1. Decrement N to N - 1
2. If a * b = N, you may decrement N to the larger of a and b

EXAMPLE:
Input: N = 100
Route: 100 -> 10 -> 9 -> 3 -> 2 -> 1
Steps: 5

EXPLANATION:
- 100 = 10 * 10, so we can go to max(10, 10) = 10
- 10 = 2 * 5, so we can go to max(2, 5) = 5, but 5-1=4 is better
- 9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 2 -> 1 (9 steps) OR
- 9 = 3 * 3, so we can go to max(3, 3) = 3
- 3 = 1 * 3, so we can go to max(1, 3) = 3 (no improvement)
- 3 - 1 = 2
- 2 - 1 = 1

INTUITION:
- This is a shortest path problem in a graph where:
  - Nodes are numbers from 1 to N
  - Edges represent valid moves (subtract 1 or factor-based moves)
- We need to find the minimum path from N to 1
- Two approaches: DFS with memoization (recursive) or BFS (iterative)
- BFS is more efficient as it explores level by level and finds shortest path first
*/

// APPROACH 1: RECURSIVE DFS WITH MEMOIZATION
/*
INTUITION:
- Try all possible moves at each step
- Use recursion to explore all paths
- Return minimum steps among all possibilities

ALGORITHM:
1. Base case: if n == 0 or n == 1, return 0
2. Try all factor pairs (a, b) where a * b = n
3. For each factor pair, recursively find steps for max(a, b)
4. Also try subtracting 1
5. Return minimum of all possibilities

TIME COMPLEXITY: O(N * sqrt(N))
- For each number n, we check factors up to sqrt(n)
- In worst case, we might explore all numbers from N down to 1
- Each number takes O(sqrt(n)) time to find factors

SPACE COMPLEXITY: O(N)
- Recursion stack depth can be up to N
- No memoization in this version (can be optimized with DP)

OPTIMIZATION: Add memoization to avoid recalculating same subproblems
*/
int minsteps(int n) {
    if (n == 0 || n == 1) {
        return 0;  // Already at 1, no steps needed
    }

    int ans = INT_MAX;

    // Try all factor-based reductions
    // Only check up to sqrt(n) as factors come in pairs
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int a = i;
            int b = n / i;
            // Take the larger factor and recursively find steps
            int steps = minsteps(max(a, b)) + 1;
            ans = min(ans, steps);
        }
    }

    // Try subtracting 1 (always a valid move)
    int count = minsteps(n - 1) + 1;
    ans = min(ans, count);

    return ans;
}

// APPROACH 2: BFS (BREADTH-FIRST SEARCH)
/*
INTUITION:
- BFS explores all nodes at current distance before moving to next level
- First node to reach 1 gives us the shortest path
- Use queue to maintain nodes to explore
- Use visited array to avoid revisiting same nodes

ALGORITHM:
1. Start BFS from N with 0 steps
2. For each node, try all possible moves:
   - Subtract 1
   - Use factor-based moves
3. Add unvisited nodes to queue with incremented step count
4. Return steps when we reach 1

TIME COMPLEXITY: O(N * sqrt(N))
- Each number is visited at most once
- For each number, we check factors up to sqrt(n)
- Total: O(N * sqrt(N))

SPACE COMPLEXITY: O(N)
- Queue can contain up to N elements
- Visited array of size N

WHY BFS IS BETTER:
- Guarantees shortest path (explores level by level)
- No recursion stack overflow for large N
- More efficient than DFS for shortest path problems
*/
int minstepsbfs(int n) {
    queue<pair<int, int>> q;  // {value, steps}
    vector<bool> vis(n + 1, false);  // Track visited nodes

    // Start BFS from n with 0 steps
    q.push({n, 0});
    vis[n] = true;

    while (!q.empty()) {
        int num = q.front().first;
        int steps = q.front().second;
        q.pop();

        // Base case: reached 1
        if (num == 1)
            return steps;

        // Try num - 1 (always a valid move)
        if (num - 1 >= 1 && !vis[num - 1]) {
            q.push({num - 1, steps + 1});
            vis[num - 1] = true;
        }

        // Try factor-based steps
        // Check all factors up to sqrt(num)
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                int a = i;
                int b = num / i;
                int next = max(a, b);  // Take larger factor
                
                // Only add if not visited and within bounds
                if (!vis[next]) {
                    q.push({next, steps + 1});
                    vis[next] = true;
                }
            }
        }
    }

    return -1;  // Unreachable case (defensive programming)
}

// OPTIMIZED VERSION WITH MEMOIZATION
/*
MEMOIZATION OPTIMIZATION:
- Cache results to avoid recalculating same subproblems
- Reduces time complexity significantly
- Space complexity increases to O(N) for cache

TIME COMPLEXITY: O(N * sqrt(N)) but with much better constant factors
SPACE COMPLEXITY: O(N) for memoization cache
*/
unordered_map<int, int> memo;

int minsteps_memoized(int n) {
    if (n == 0 || n == 1) {
        return 0;
    }
    
    // Check if already calculated
    if (memo.find(n) != memo.end()) {
        return memo[n];
    }

    int ans = INT_MAX;

    // Try factor-based moves
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int a = i;
            int b = n / i;
            int steps = minsteps_memoized(max(a, b)) + 1;
            ans = min(ans, steps);
        }
    }

    // Try subtracting 1
    int count = minsteps_memoized(n - 1) + 1;
    ans = min(ans, count);

    // Cache the result
    memo[n] = ans;
    return ans;
}

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;
    
    cout << "Recursive approach: " << minsteps(n) << endl;
    cout << "BFS approach: " << minstepsbfs(n) << endl;
    cout << "Memoized approach: " << minsteps_memoized(n) << endl;
    
    return 0;
}

/*
COMPARISON OF APPROACHES:

1. RECURSIVE DFS:
   - Pros: Simple to understand, easy to implement
   - Cons: Can cause stack overflow, recalculates subproblems
   - Best for: Small inputs, educational purposes

2. BFS:
   - Pros: Guarantees shortest path, no stack overflow, efficient
   - Cons: Slightly more complex implementation
   - Best for: Production code, large inputs

3. MEMOIZED DFS:
   - Pros: Combines simplicity of recursion with efficiency
   - Cons: Uses more memory for cache
   - Best for: When you need both simplicity and efficiency

TESTING:
Try these test cases:
- N = 10: Should return 3 (10 -> 5 -> 4 -> 3 -> 2 -> 1)
- N = 100: Should return 5 (100 -> 10 -> 9 -> 3 -> 2 -> 1)
- N = 1: Should return 0
- N = 2: Should return 1

EDGE CASES:
- N = 1: Already at target
- N = 2: Only option is subtract 1
- Prime numbers: Only option is subtract 1 repeatedly
- Perfect squares: Can use factor-based moves
*/