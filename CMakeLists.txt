cmake_minimum_required(VERSION 3.9)
project(Encryption_algorithms)

set(CMAKE_CXX_STANDARD 11)

add_executable(Encryption_algorithms main.cpp

        enc_algortihms/PlayFair/PlayFair.h
        enc_algortihms/PlayFair/PlayFair.cpp

        enc_algortihms/Crypto/Crypto.h
        enc_algortihms/Crypto/Crypto.cpp

        enc_algortihms/RailFenceTrans/railFenceTrans.cpp
        enc_algortihms/RailFenceTrans/railFenceTrans.h

        enc_algortihms/RSA/RSA.h

        UTIL/BigInt.h
        UTIL/conversion.h
        enc_algortihms/keyless/Keyless.h enc_algortihms/keyless/Keyless.cpp enc_algortihms/keyless/Keyless.h)


