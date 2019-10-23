lines = open("words_big", "r").readlines()
d = {}

for line in lines:
   _tmp = str(line).replace("\n", "")

   if _tmp not in d:
      d[_tmp] = 0
   else:
      d[_tmp] += 1