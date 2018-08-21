
# What about negatives
# what about empty lists
def cum_sum(l: list):
	return [sum(l[0:i + 1]) for i in range(len(l))]


# Testing for the 1st Function
print(cum_sum([]))
print(cum_sum([1]))
print(cum_sum([-1,1]))
print(cum_sum([1,2,3,4]))



def l_str_sort(l: list):
	return sorted(l, key=lambda x: len(x))


# Testing for the 2nd Function
print(l_str_sort( [ "python" , "perl" , "java" , "c", "haskell", "ruby"]))


def l_unique(l: list, k=lambda x: x):
	return list(set(map(k, l)))


# Testing for the 3rd Function
print(l_unique( [ "python", "java", "Python", "Java" ] , k= str.lower ))
print(l_unique( [ "python", "java", "Pytho1", "Java" ] ))


def line_wrap(line: str, line_len: int):
	return "\n".join([line[i:i+line_len] for i in range(0, len(line), line_len)])


# Testing for the 3rd Function
print(line_wrap("We passed in a user-defined function applied to each item in the list.", 10))