import subprocess
import csv

def run_filesec(times, mode, input_file, output_file):
    with open(output_file, 'w', newline='') as csvfile:
        # fieldnames = ['Read calls', 'Write calls', 'Elapsed time']
        fieldnames = ['Elapsed time (Seconds)']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

        for i in range(times):
            result = subprocess.run(['./filesec', mode, input_file], capture_output=True, text=True)
            output = result.stdout.strip().split('\n')
            
            # read_calls = int(output[0].split(': ')[1])
            # write_calls = int(output[1].split(': ')[1])
            elapsed_time = float(output[2].split(': ')[1].split(' ')[0])

            # writer.writerow({'Read calls': read_calls, 'Write calls': write_calls, 'Elapsed time': elapsed_time})
            writer.writerow({'Elapsed time (Seconds)': elapsed_time})

if __name__ == '__main__':
    # Number of times to run the C program
    times = int(input("Enter the number of times to run the C program: "))

    # Mode: "-e" or "-d"
    mode = input("Enter the mode (-e or -d): ")

    # Input file
    input_file = input("Enter the input file name: ")

    # Output CSV file
    output_file = "output.csv"

    run_filesec(times, mode, input_file, output_file)
