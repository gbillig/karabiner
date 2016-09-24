# Karabiner

Karabiner is a free and open source password manager.

Today we are expected to manage dozens of accounts and remember many different passwords. Additionally, it is recommended not to reuse passwords because if a single password is revealed to the public, anyone may gain access to your other accounts. Karabiner makes it simple for you to securely store all your passwords in an encrypted database.

The database is encrypted with a master password. This master password is the only one that you need to remember in order to access all your account information. The master password is salted and then hashed using the SHA-256 hash function. The resulting key is then used to encrypt the account data using the AES-256 block cipher and the CBC mode of operation.

The cryptographic functions are implemented by the glbcrypto library, available [here](https://github.com/gbillig/glbcrypto). 

![Karabiner main window](https://raw.githubusercontent.com/gbillig/karabiner/master/doc/karabiner.png)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development.
### Prerequisities

To build this project, you will need to have the Qt5 framework installed on your system. You may obtain a copy from their official website [www.qt.io](https://www.qt.io).

### Installing

To build this project, you need to run the build script.

```
bash build.sh
```
Alternatively, if you use the Qt-Creator for development, you may load the karabiner.pro file and build the project from within the IDE.

## Running

After you successfully built the project, you will find the Karabiner binary in the build directory. You can execute it to use the program.

## Built With

* glbcrypto - A library that implements cryptographic functions. Available [here](https://github.com/gbillig/glbcrypto).
* Qt - A cross-platform application framework. Available [here](https://www.qt.io).
* Atom - ergaerga

## Authors

* **Gleb Billig** - *Initial work* - [gbillig](https://github.com/gbillig)

## License

This project is licensed under the LGPLv3 License - see the [LICENSE.md](LICENSE.md) file for details
