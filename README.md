# spell_checker
Get dictionary words from first parameter and read them into tree.

Todo tree:
  - find if next letter already exists within subtree and link if so
    - thoughts on how to do this: 
      - keep list of predeceeding letters that will make it legal
      - b --> i --> t* --> i --> n --> g*
      -  \\--> o ---/
      -  here, bit, biting, and bot are words but 'boting' should not be a word
  - reorder tree according to frequency of letters searched letters more popular are shifted to the front
  - give recommendations if word is mispelled (list words that are ~1 letter different than original)
  - write sentence using most frequently used words
  - parallelize
