s = "756e50523437684845414a3561733951377a7143574e6758354a35686e47587348336750664b394d"

def get_readable_stack_dump(stack_dump: str):
    group_16 = [s[i:i+16] for i in range(0, len(s), 16)]
    final_list = []
    for group in group_16:
        pair = [group[i:i+2] for i in range(0, len(group), 2)]
        final_list += pair[::-1]
    return final_list

if __name__ == "__main__":
    ascii_pass = ''.join([chr(int(h, 16)) for h in get_readable_stack_dump(s)])
    print(ascii_pass)