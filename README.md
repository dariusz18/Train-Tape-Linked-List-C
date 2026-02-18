# Train Tape — Doubly Linked List with Command Queue in C

Implementation of a **Turing machine-inspired tape** using a **circular doubly linked list** in C, combined with a **doubly linked command queue**. Developed as a Data Structures course assignment.

## Overview

The data structure simulates a train of wagons on a circular tape. A special pointer (`mecanic`) acts as the read/write head, moving left or right along the wagons. Commands are not executed immediately — they are enqueued and executed one at a time via an `EXECUTE` instruction. A `SWITCH` command reverses the queue order in-place.

## Data Structures

### Train (Circular Doubly Linked List)
- Each wagon (`TrainNode`) holds a `char` value and `prev`/`next` pointers
- A **sentinel node** (`'\0'`) serves as the list boundary — it is never a real wagon
- The `mecanic` pointer marks the currently active wagon (read/write head)
- The train is initialized with a single `'#'` wagon

### Command Queue (Doubly Linked Queue)
- Stores pending commands as strings
- Supports `enqueue`, `dequeue`, and `switch_q` (in-place reversal)
- Commands are dequeued and executed one at a time via `EXECUTE`

## Supported Commands

| Command | Description |
|---|---|
| `MOVE_LEFT` | Move `mecanic` one wagon to the left (wraps if at sentinel) |
| `MOVE_RIGHT` | Move `mecanic` one wagon to the right; appends `'#'` if at end |
| `WRITE <c>` | Write character `c` into the current wagon |
| `INSERT_LEFT <c>` | Insert a new wagon to the left of `mecanic`; ERROR if at first position |
| `INSERT_RIGHT <c>` | Insert a new wagon to the right of `mecanic` |
| `CLEAR_CELL` | Remove the current wagon; handles first, middle, and last cases |
| `CLEAR_ALL` | Remove all wagons except the first; reset to `'#'` |
| `SEARCH <str>` | Circular search for substring from current position; ERROR if not found |
| `SEARCH_LEFT <str>` | Search leftward from current position; ERROR if not found |
| `SEARCH_RIGHT <str>` | Search rightward from current position; ERROR if not found |
| `EXECUTE` | Dequeue and execute the front command |
| `SWITCH` | Reverse the command queue in-place |
| `SHOW` | Print the full train, marking `mecanic` with `\|c\|` |
| `SHOW_CURRENT` | Print only the value of the current wagon |

## Implementation Details

### `move()`
`MOVE_LEFT` moves `mecanic` to the previous node, skipping the sentinel if needed. `MOVE_RIGHT` moves to the next node, or appends a new `'#'` wagon via `addVagon()` if already at the end.

### `decuplare()` — CLEAR_CELL
Handles three cases: mecanic at the first wagon, in the middle, or at the last wagon. Pointers are rewired and the node is freed. After deletion, `mecanic` is repositioned to the nearest remaining wagon.

### `clear()` — CLEAR_ALL
Frees all wagons except the first, resets its value to `'#'`, and repositions `mecanic` and sentinel links.

### `cuplare()` — INSERT_LEFT / INSERT_RIGHT
Inserts a new wagon adjacent to `mecanic` and moves `mecanic` to the new node. `INSERT_LEFT` returns `ERROR` if `mecanic` is the first wagon (no room to insert to the left).

### `search()`
Performs a circular substring search starting from `mecanic`, using a counter-based approach. When a mismatch occurs mid-match (counter > 0), `mecanic` advances one step and the partial match restarts — this handles overlapping patterns. If found, `mecanic` is moved back to the first character of the match.

### `search_directie()` — SEARCH_LEFT / SEARCH_RIGHT
Builds a temporary string from the current position in the chosen direction, then uses `strncmp` to find the substring. `mecanic` is repositioned using an offset counter.

### `switch_q()`
Reverses the command queue by swapping all `next`/`prev` pointers in a single pass, then swapping `head` and `tail`.

## Building & Running

```bash
make
./tema1
```

Input is read from `tema1.in`, output is written to `tema1.out`.

### Input Format

```
<number_of_commands>
<command_1>
<command_2>
...
```
