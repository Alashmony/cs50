import re
# from math import floor

text = input("Text: ")
letts = 0
words = 1
sents = 0

sen_end = [".", "?", "!"]

for i in range(len(text)):
    # end of word
    if text[i] == " ":
        words += 1
    # end of sentence
    elif text[i] in sen_end:
        sents += 1
    elif re.match("[a-zA-Z]", text[i]):
        letts += 1

l_ = (letts / words) * 100
s_ = (sents / words) * 100

col_liau = int(round((0.0588 * l_) - (0.296 * s_) - 15.8, 0))

if col_liau < 1:
    print("Before Grade 1")
elif col_liau > 16:
    print("Grade 16+")
else:
    print(f"Grade {col_liau}")
