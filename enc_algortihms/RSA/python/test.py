p = 1740948824551753
q = 1826603801251001
e = 151
n = p * q
print(n)
phi = (p - 1) * (q - 1)
print(phi)


# Took from SO
#  a*x + b*y =gcd
def extended_gcd(a, b):
    if b == 0:  # check on a for the zero division
        return a, 1, 0  # a*1 + b *0 = gcd(a,0) =  a
    g, x, y = extended_gcd(b, a % b)
    return g, y, x - (a // b) * y


# since a*x + b*y =gcd
# since e*x + phi*y =1 take mod phi
# (e*x) mod phi = 1
def multiplicative_inverse(e, phi):
    g, x, y = extended_gcd(e, phi)
    if g != 1:  # they aren't coprime
        raise Exception('No modular inverse')
    return x #% phi  # mod phi just for optimization


print(multiplicative_inverse(e, phi))
