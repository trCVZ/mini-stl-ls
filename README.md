# Mini LS (C++23)

A simple clone of `ls` written in C++23 using only the STL (`std::filesystem`).

## Build

```bash
make build
````

or manually:

```bash
clang++ -Wall -Wextra -std=c++23 ls.cpp -o ls.exe
```

On Linux/macOS:

```bash
clang++ -Wall -Wextra -std=c++23 ls.cpp -o ls
```

## Options

* `-l` : print one entry per line
* `-a` : show hidden files (starting with `.`)
* `--` : end of options (required when a directory starts with `-`)


## Example directory structure

```
project/
├── ls.cpp
├── Makefile
├── ls.exe
│
├── folder1/
│   ├── a.txt
│   ├── b.txt
│
├── folder2/
│   ├── a.txt
│   ├── b.txt
│
├── -folder/
│   ├── hidden.txt
│   ├── data.cpp
```

## Working examples

```bash
./ls.exe
./ls.exe -l -a
./ls.exe folder1
./ls.exe -l folder1
./ls.exe -a folder1
./ls.exe -l -a folder2
./ls.exe -l -a -- -folder
./ls.exe -- -folder
```

## Non-working or invalid examples

```bash
./ls.exe -x folder
./ls.exe folder1 folder2
./ls.exe -l -a -folder
```

## Notes

* If no directory is provided, `.` (current directory) is used.
* A directory name starting with `-` must be placed after `--`.
* Example:

```bash
./ls.exe -l -a -- -folder
```

* Without `--`, `-folder` is interpreted as an invalid option.
