# Encryption_algorithms_Task

This is my personal assignment for cryptography task.

## RSA

Contains modest biginteger implementation using vectors as blocks of numbers, I tried multiplying numbers as repeated addition in logarithmic complexity(divide and conquer) of the number space :D 
as well as modular exponentiation as repeated multiplcation with logarithmic complexity (divide and conquer), divion as binary search :D also as (divide and conquer).. that was fun :D .

I also implemented the extended GCD and modulo-inverse for establishing RSA channel.

```
====================================================Internal Values=====================================================
p: 874784103708338091190147416073637
q: 861882254470807093282520399722559
n: 753960895479366753745699328782844487082220405597445665431914077083
phi: 753960895479366753745699328782842750415862226452261192764098280888
e: 2879
d: 365064775372781262494444204349872453657419917914016013446736020687
=======================================================Vocabulary=======================================================
_ !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^`abcdefghijklmnopqrstuvwxyz{|}~

Enter message
mohammed alaa el kOmy 012XX
mohammed alaa el kOmy 012XX
=======================================================Plain Text=======================================================
Text: mohammed alaa el kOmy 012XX
Num: 205123075875681986345349980530590985105852103077517197
====================================================Public Key Pair=====================================================
e: 2879
n: 753960895479366753745699328782844487082220405597445665431914077083
====================================================Private Key Pair====================================================
d: 365064775372781262494444204349872453657419917914016013446736020687
n: 753960895479366753745699328782844487082220405597445665431914077083
=======================================================(E*D)%Phi========================================================
Euler Identity: (E*D)%Phi=1
======================================================Cipher Text=======================================================
Text: "{k/363W_gX7L-^$ Cgj!BTggEJYmAL[y5
Num: 729887962716545365379008340159555160143238381451166716115476149977
========================================================Restored========================================================
mohammed alaa el kOmy 012XX
```
## PlayFair

Implemented as 2 indexed data structures for fast retrieval.

## rail-fence cipher 
Implemented efficiently by applying an index.

## Resources

* [blog post part 1](http://doctrina.org/How-RSA-Works-With-Examples.html)
* [blog post part 2](http://doctrina.org/Why-RSA-Works-Three-Fundamental-Questions-Answered.html)
* [RSA - Maths - youtube](https://www.youtube.com/watch?v=EOhLZRwxaVo)
* [RSA-wiki](https://en.wikipedia.org/wiki/RSA_(cryptosystem))
* [Euler-Totient_Function-wiki](https://en.wikipedia.org/wiki/Euler%27s_totient_function)
* [primality-test-miller-rabin](https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/)
* [big int CF](https://gist.github.com/ar-pa/957297fb3f88996ead11)
* [PlayFair wiki](https://en.wikipedia.org/wiki/Playfair_cipher)

* [Rail_fence wiki](https://en.wikipedia.org/wiki/Rail_fence_cipher)

