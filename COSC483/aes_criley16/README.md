# Advanced Encryption Standard (AES) Project
## How to compile and run
1. Unzip the file (unzips as folder named **aes_criley16** conatains **aes.cpp** and **appendix_c.txt**):
    `unzip aes_criley16.zip`

2. Change directory 
     `cd aes_criley16`

3. Compile 
    `g++ -o aes aes.cpp`

4. running the program
    - run with output to console: `./aes`
    - run with output redirected to out.txt and then comparing output to appendix_c.txt `./aes > out.txt && diff -s out.txt appendix_c.txt`

## Resources used
- https://www.usenix.org/legacy/publications/library/proceedings/cardis02/full_papers/valverde/valverde_html/ used for understanding mixcolumns

## Test Cases
- passed all test cases in appendix_c