# spell_checker
Get dictionary words from first parameter and read them into tree.

Todo tree:
  - find if next letter already exists within subtree and link if so
  - implement faster insert and search (linear search through n linked lists- very slow with lots of words)
  - keep track of n heads of subtrees without keeping n pointers so I can combine insert_letter and find_letter
  - check spelling of another text file or stdin
  - parallelize
