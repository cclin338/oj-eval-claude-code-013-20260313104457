# OJBench Problem 013 - STLite Map Solution Summary

## Task Overview
Implement a C++ STL-like `map` data structure with a self-balancing binary search tree, supporting all standard map operations including iterators.

## Solution Approach

### Data Structure Choice
**Red-Black Tree** - A self-balancing binary search tree that guarantees O(log n) time complexity for insertions, deletions, and lookups.

### Key Implementation Details

1. **Node Structure**
   - Each node contains a pointer to the key-value pair
   - Left/right child pointers and parent pointer for bidirectional traversal
   - Color field (RED or BLACK) for Red-Black Tree balancing

2. **Core Operations**
   - `insert()`: Insert with Red-Black Tree fixup
   - `erase()`: Delete with Red-Black Tree fixup
   - `find()`: Binary search tree lookup
   - `at()`: Bounds-checked access
   - `operator[]`: Insert-or-access operation

3. **Iterator Implementation**
   - Bidirectional iterators using tree successor/predecessor operations
   - Proper handling of `begin()`, `end()`, `++`, `--` operations
   - Support for both `iterator` and `const_iterator`

4. **Memory Management**
   - Deep copy in copy constructor and assignment operator
   - Proper cleanup in destructor
   - No memory leaks (verified by memcheck tests)

### Red-Black Tree Balancing
- Maintains the following properties:
  1. Every node is either RED or BLACK
  2. Root is always BLACK
  3. All leaves (NULL) are BLACK
  4. RED nodes cannot have RED children
  5. All paths from root to leaves have the same number of BLACK nodes

## Test Results

### Local Testing
- ✅ Test one: PASSED
- ✅ Test two: PASSED
- ✅ Test three: PASSED
- Tests four and five: Not fully tested locally (too large)

### Online Judge Results

#### Problem 2671 (Basic Functionality)
- **Score**: 100/100
- **Submission ID**: 752557
- **Status**: Accepted
- **All test groups**: PASSED (including memory leak checks)

Test groups breakdown:
- One: 10/10
- One.memcheck: 10/10
- Two: 10/10
- Two.memcheck: 10/10
- Three: 10/10
- Three.memcheck: 10/10
- Four: 10/10
- Four.memcheck: 10/10
- Five: 10/10
- Five.memcheck: 10/10

#### Problem 2672 (Performance Tests)
- **Score**: 100/100
- **Submission ID**: 752558
- **Status**: Accepted
- **Benchmarks**: All PASSED

Test groups breakdown:
- Extra corner tests: 30/30
- Benchmark: Insert: 10/10 (54.77% speed of std::map)
- Benchmark: Erase: 10/10 (50.11% speed of std::map)
- Benchmark: Travel: 10/10 (62.95% speed of std::map)
- Benchmark: Find: 10/10 (~100% speed of std::map)
- Benchmark: Min & Max: 10/10 (2.94% speed - could be optimized)
- Benchmark: Operator []: 10/10 (71.03% speed of std::map)
- Benchmark: Copy & Clear: 10/10 (33.66% speed of std::map)

## Resource Usage
- **Total Submissions**: 2 out of 6 allowed
- **Final Status**: Both problems solved with perfect scores

## Performance Notes
The implementation passes all performance tests, though there's room for optimization:
- Find operations perform nearly as well as std::map
- Begin/end operations could be optimized by caching min/max nodes
- Overall performance is acceptable for all practical use cases

## Compliance
- ✅ Only allowed headers used: `<cstddef>`, `<functional>`, `<cstring>`, `<iostream>`, `<cmath>`, `<string>`
- ✅ No default constructor assumption for template types
- ✅ Proper exception handling
- ✅ No memory leaks
- ✅ Interface specifications preserved

## Conclusion
Successfully implemented a fully functional STL-compatible map data structure using Red-Black Trees, achieving perfect scores on both Online Judge problems with only 2 out of 6 allowed submissions used.
