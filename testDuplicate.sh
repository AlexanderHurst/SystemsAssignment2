#!/bin/bash

rm foobar
rm bizbaz

echo
echo executing duplicate with no foobar file, expect read error
./duplicate foobar bizbaz

touch foobar
chmod a-r foobar

echo
echo
echo executing duplicate with no read permissions on foobar, expect read error
./duplicate foobar bizbaz

chmod a+r foobar
echo test test data data > foobar

touch bizbaz
chmod a-w bizbaz

echo
echo
echo executing duplicate with no write permissions on bizbaz, expect write error
./duplicate foobar bizbaz

chmod a+w bizbaz

echo
echo
echo executing duplicate with all permissions on all files, expect no errors
./duplicate foobar bizbaz

echo
echo running cat foobar
cat foobar
echo running cat bizbaz, output should be the same
cat bizbaz