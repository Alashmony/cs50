while True:
    number = input("Number: ")
    try:
        int(number)
        break
    except:
        pass

number = number.strip()
num_len = len(number)
first_two = int(number[:2])

# print(first_two,num_len)


def valid(number, num_len):
    total = 0
    for i in range(num_len):
        # current number
        current_location = int(number[num_len - i - 1])
        # if the location is even as i is a zero indexed
        if i % 2 == 0:
            total += current_location
        else:
            #  If double the current number is less than 10
            if current_location * 2 < 10:
                total += (current_location * 2)
            else:
                # Sum of the two digits
                summed = (current_location % 10) + 1
                total += summed
    # print(total)
    if total % 10 == 0:
        return True
    else:
        return False


valid_number = valid(number, num_len)

if num_len == 15 and first_two in [34, 37]:
    print("AMEX")

elif (num_len == 16) and (first_two in [51, 52, 53, 54, 55]):
    print("MASTERCARD")

elif (num_len in [13, 16]) and (first_two >= 40 and first_two < 50) and valid_number:
    print("VISA")

else:
    print("INVALID")
