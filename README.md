<h1 align="center">Blockchain</h1>

<p align="center">
<img width="" height="" src="./media/vecteezy_concepto-de-red-de-cadena-de-bloques-tecnologia-de_6514761.jpg">
</p>


<h2 align="center"><a href="https://github.com/crasride/holbertonschool-blockchain/tree/main/crypto"> 👉 1. Crypto</a></h2>


### Learning Objectives

<details>
<summary>How a Blockchain is considered “unbreakable”</summary>
<br>

* Blockchains are considered secure due to their decentralized and distributed nature. The use of cryptographic algorithms and consensus mechanisms makes it extremely difficult to tamper with the data stored in blocks.

</details>

<details>
<summary>What is a hash algorithm</summary>
<br>

* A hash algorithm is a mathematical function that takes an input (or message) and produces a fixed-size string of characters, which is typically a hash value. It is used in blockchain to create a unique identifier for data stored in a block.

</details>

<details>
<summary>What SHA stands for</summary>
<br>

* SHA stands for Secure Hash Algorithm. It is a family of cryptographic hash functions designed by the National Security Agency (NSA). SHA-256 and SHA-3 are examples of widely used hash functions in blockchain.

</details>

<details>
<summary>How hash algorithms apply to Blockchains</summary>
<br>

* Hash algorithms in blockchain are used to create a fixed-size representation (hash) of the block's data. This hash is included in the block, and any change in the data will result in a completely different hash, ensuring data integrity.

</details>

<details>
<summary>What is asymmetric cryptography</summary>
<br>

* Asymmetric cryptography involves the use of key pairs: a public key and a private key. Data encrypted with the public key can only be decrypted with the corresponding private key, and vice versa.

</details>

<details>
<summary>How asymmetric cryptography applies to cryptocurrencies</summary>
<br>

* Cryptocurrencies use asymmetric cryptography for secure transactions. Users have a pair of cryptographic keys, and the public key serves as their address. Transactions are signed with the private key to verify ownership.

</details>

<details>
<summary>What ECC stands for</summary>
<br>

* ECC stands for Elliptic Curve Cryptography, which is a type of asymmetric cryptography. It provides strong security with shorter key lengths compared to traditional cryptographic methods.

</details>

<details>
<summary>What ECDSA stands for</summary>
<br>

* ECDSA stands for Elliptic Curve Digital Signature Algorithm. It is an algorithm used in blockchain and cryptocurrencies for generating digital signatures, providing authentication and data integrity.

</details>

<details>
<summary>What a digital signature is</summary>
<br>

* A digital signature is a cryptographic technique that verifies the authenticity and integrity of a message or document. In the context of cryptocurrencies, it ensures that a transaction was authorized by the rightful owner of the private key.

</details>

<details>
<summary>How digital signatures apply to cryptocurrencies</summary>
<br>

* Digital signatures in cryptocurrencies are generated using the private key of the sender to prove ownership and authorization of a transaction. Verifying the signature with the corresponding public key confirms the authenticity of the transaction.

</details>
<br>


<h2 align="center"><a href="https://github.com/crasride/holbertonschool-blockchain/tree/main/blockchain/v0.1"> 👉 2. Data structures</a></h2>

### Learning Objectives

<details>
<summary>How a Blockchain is structured</summary>
<br>

* A blockchain is a distributed and decentralized ledger that records transactions across a network of computers.
* It consists of a chain of blocks, where each block contains a list of transactions.

</details>


<details>
<summary>How Blocks refer to each other</summary>
<br>

* Each block in a blockchain contains a reference (usually a hash) to the previous block in the chain.
* This creates a linked structure, where blocks are connected in a sequential order.

</details>

<details>
<summary>How immutability is maintained in a Blockchain</summary>
<br>

* Immutability in a blockchain is maintained through the use of cryptographic hashes.
* Once a block is added to the blockchain, it is difficult to alter because changing the data in one block would require changing the hash of that block and all subsequent blocks.

</details>

<details>
<summary>What is the Genesis Block, and why is it important</summary>
<br>

* The Genesis Block is the first block in a blockchain.
* It serves as the foundation for the entire blockchain and has no predecessor.
* The information in the Genesis Block is usually hardcoded into the blockchain's protocol.

</details>

<details>
<summary>How to hash a Block, and what information is stored in the hash</summary>
<br>

* A block is hashed by applying a cryptographic hash function (e.g., SHA-256) to its contents.
* The hash includes the block's data, timestamp, previous block's hash, and a nonce (a value that, when hashed, meets certain criteria, typically related to the proof-of-work mechanism).
* The hash uniquely identifies the block and ensures its integrity.

**Information Stored in the Hash:**

* The hash of a block is a fixed-length string of characters that uniquely represents the block.
* It includes information such as the block's data, timestamp, previous block's hash, and nonce.
* Any change in the block's content will result in a completely different hash.

</details>
<br>



<h2 align="center"><a href="https://github.com/crasride/holbertonschool-blockchain/tree/main/blockchain/v0.2"> 👉 3. Block Mining</a></h2>

### Learning Objectives

<details>
<summary>What is the purpose of the Block difficulty</summary>
<br>

The block difficulty in a blockchain network serves the purpose of regulating the rate at which new blocks are added to the blockchain. It is a measure of how difficult it is to find a hash below a certain target value. 

This difficulty adjustment ensures that new blocks are added at a relatively constant rate, regardless of changes in the network's total hash rate.

</details>


<details>
<summary>What is the Proof of Work</summary>
<br>

Proof of Work is a consensus mechanism used in blockchain networks, most notably in `Bitcoin`.

It involves participants (miners) solving complex mathematical puzzles to validate and process transactions on the network. 

 Miners compete to find a solution to a cryptographic puzzle, which requires significant computational power. Once a miner finds a solution, they can add a new block to the blockchain and are rewarded with newly created cryptocurrency (like `Bitcoin`).

</details>

<details>
<summary>How to check that a given hash matches a given difficulty</summary>
<br>

To check if a given hash matches a given difficulty, you need to compare the hash against a target value. The difficulty level determines the target value, which is essentially a threshold that the hash must be below for the block to be considered valid. 

If the hash is below the target value, it meets the difficulty requirement. Miners repeatedly hash block headers with different nonce values until they find a hash that meets the required difficulty.


</details>
</details>

<details>
<summary>How to mine a Block</summary>
<br>

* Mining a block involves the following steps:

	* Collecting transactions: Gather unconfirmed transactions from the network.
	* Building a block: Group these transactions into a block along with other data such as a reference to the previous block (blockchain's history), a timestamp, and a nonce.
	* Finding a valid hash: Use computational power to repeatedly hash the block header with different nonce values until a hash is found that meets the current difficulty requirement.
	* Propagating the block: Once a valid hash is found, propagate the block to other nodes in the network for validation and inclusion in the blockchain.

</details>

<details>
<summary>Why Block mining is brute force only</summary>
<br>

Block mining is often referred to as brute force because it involves trying numerous combinations of nonce values until a valid hash is found. 

There's no shortcut to finding the correct hash other than iterating through different nonce values until one produces a hash that meets the difficulty requirement. 

This process requires significant computational power and energy consumption.


</details>

<details>
<summary>How to automatically adjust the difficulty</summary>
<br>

Difficulty adjustment is typically built into the protocol of blockchain networks. In `Bitcoin`, for example, the difficulty adjusts every 2016 blocks (approximately every two weeks). 

The adjustment is based on the time it took to mine the previous 2016 blocks. If blocks were mined faster than the target block time (10 minutes in Bitcoin's case), the difficulty increases; if they were mined slower, the difficulty decreases.

This mechanism aims to maintain a relatively stable block production rate regardless of fluctuations in the network's hash rate.

</details>
<br>

<h2 align="center"><a href=""> 👉 4. Transactions</a></h2>

* in progress

<h2 align="center"><a href=""> 👉 5. CLI</a></h2>

* in progress


## Author

* José Fernàndez Armas

<sub>_You can contact me_ 📩

[Fernandez Armas José](https://github.com/crasride)

<p align="left">
<a href="https://twitter.com/JosFern35900656" target="blank"><img align="center" src="./media/twitter.svg" alt="crasride" height="30" width="40" /></a>
<a href="https://www.linkedin.com/in/jd-fernandez/" target="blank"><img align="center" src="./media/linked-in-alt.svg" alt="crasride" height="30" width="40" /></a>
<a href="https://medium.com/@4990" target="blank"><img align="center" src="./media/medium.svg" alt="@crasride" height="30" width="40" /></a>
<a href="https://discord.gg/José Fernandez Armas#7992" target="blank"><img align="center" src="./media/discord.svg" alt="crasride" height="30" width="40" /></a>
</p>
