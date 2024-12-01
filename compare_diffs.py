import os
for file in  os.listdir("alunodetail"):
    if file.endswith("tab.diff"):
        print(f"\033[93m{file}\033[0m")
        with open(os.path.join("alunodetail", file), 'r') as f:
            lines = f.readlines()
            coming, going = set(), set()
            for line in lines:
                if line.startswith(">"):
                    going.add(line[2:])
                if line.startswith("<"):
                    coming.add(line[2:])
            if coming == going:
                print("\033[92mOK\033[0m")
            else:
                print("\033[91mreview me\033[0m")
                test1 = coming - going
                test2 = going - coming
                print("coming:", coming)
                print("going:", going)
                for item in test1:
                    print(item)
                print("*************")
                for item in test2:
                    print(item)
        
