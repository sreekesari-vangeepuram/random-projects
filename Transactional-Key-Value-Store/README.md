# Transactional Key-value Store in Go

A basic transactional key-value store implemented in Golang.
All the data store in this store is non-persistent.

This project is a [blog post](https://www.freecodecamp.org/news/design-a-key-value-store-in-go/) by [freecodecamp](https://www.freecodecamp.org/).

Usage:

Command,Description
SET,Sets the given key to the specified value. A key can also be updated.
GET,Prints out the current value of the specified key.
DELETE,Deletes the given key. If the key has not been set, ignore.
COUNT,Returns the number of keys that have been set to the specified value. If no keys have been set to that value, prints 0.
BEGIN,Starts a transaction. These transactions allow you to modify the state of the system and commit or rollback your changes.
END,Ends a transaction. Everything done within the "active" transaction is lost.
ROLLBACK,Throws away changes made within the context of the active transaction. If no transaction is active, prints "No Active Transaction".
COMMIT,Commits the changes made within the context of the active transaction and ends the active transaction.


Small example:

```console
$ ./main
> BEGIN
> SET X 2021
> GET X
2021
> COMMIT
> GET X
2021
> SET Y 2020
> SET Z 2019
> GET Y
2020
> STOP
```

# License
Release Under [MIT License](https://github.com/sreekesari-vangeepuram/random-projects/blob/main/LICENSE)
