import subprocess

Solutions=["Solution1", "Solution2", "Solution3", "Solution4"]
Debug_N = ["10", "100", "1000"]
Debug_M = ["10"]
Release_N = ["1000000", "10000000", "100000000"]
Release_M = ["1000000"]

repeated_times = 5

def check():
    for sln in Solutions:
        for m in Debug_M:
            for n in Debug_N:
                for i in range(repeated_times):
                    subprocess.run(["./main.d",n, m, sln],check=True)

def perfomance():
    for sln in Solutions:
        for m in Release_M:
            for n in Release_N:
                for i in range(repeated_times):
                    subprocess.run(["./main",n, m, sln],check=True)

    
if __name__ == "__main__":
    # check()
    perfomance()