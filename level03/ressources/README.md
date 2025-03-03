# level03

In this binary, we can see that the **encrypted** password is stored in the `.rodata`, and is decrypted with XOR on each character against a constant. The result should be the string `Congratulations!`, also stored in the `.rodata`. To resolve this challenge, we can use the commutative and reversible property of XOR. We can XOR each character of the cyphertext **Q}|u`sfg~sf{}|a3** against the plaintext **Congratulations!** to obtain the key :

```python
cyphertext = 'Q}|u`sfg~sf{}|a3'
plaintext = 'Congratulations!'

xor_values = []

for i in range(len(cyphertext)):
    xor_value = ord(cyphertext[i]) ^ ord(plaintext[i])
    xor_values.append(xor_value)

print(xor_values)

if all(x == xor_values[0] for x in xor_values):
    password = 322424845 - xor_values[0]
    print(f"The key used for XOR encryption is: {xor_values[0]}")
    print(f"The password is : {password}")
```

We can then substract the key from the constant value used in the program. Launching this script yield to the password **322424827**.