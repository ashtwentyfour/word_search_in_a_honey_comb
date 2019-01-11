Compile:
g++ -Wall -Werror *.cpp

Run:

./a.out honeycomb.txt wordlist.txt

honeycomb.txt specifications:

1st line -> Number of layers (N)
2nd line -> Single character
3rd line -> 6 chars
4th line -> 12 chars
...
...
Nth line -> No of chars in N-1 th line + 6 chars
