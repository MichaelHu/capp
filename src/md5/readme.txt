## From 
http://tools.ietf.org/html/rfc1321

## Test suite

The MD5 test suite (driver option "-x") should print the following results:

MD5 test suite:
MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = d174ab98d277d9f5a5611c2c9f419d9f
MD5 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = 57edf4a22be3c955ac49da2e2107b67a

## How to Use

hudamin@local:md5$ ./mddriver.out -t
MD5 time trial. Digesting 1000 1000-byte blocks ... done
Digest = 4e79096e9f34094d3a8d402c014ab485
Time = 0 seconds
Floating point exception: 8

hudamin@local:md5$ ./mddriver.out -shudamin
MD5 ("hudamin") = f3e9e0bddb9cdb8111c7e48aca05ad52

hudamin@local:md5$ ./mddriver.out md5c.c
MD5 (md5c.c) = 5b2a69a52ce61ab04165d01b119379a1
