import os
import sys
import json


def parse_filename(filename):
    tokens = filename[:-4].split('-')
    test_type = tokens[0]
    theta_s = int(tokens[1][3:])
    theta_c = int(tokens[2][3:])
    r_s = int(tokens[3][3:])
    r_c = int(tokens[4][3:])
    return (test_type, theta_s, theta_c, r_s, r_c)

def main(timestamp):
    results_dir = "results/" + timestamp
    list_files = os.listdir(results_dir)

    # Read results
    results = {}
    for test_type in ["fixed_rc", "fixed_rs"]:
        results[test_type] = {}
        for theta_s in [60, 90, 120]:
            results[test_type][theta_s] = {}

    for filename in list_files:
        filepath = os.path.join(results_dir, filename)
        print(filepath)
        try:
            test_type, theta_s, theta_c, r_s, r_c = parse_filename(filename)
        except:
            continue
        with open(filepath, 'r') as f:
            lines = f.readlines()
            print(lines)
            row = lines[-1]
            row = row[row.find(',') + 1 : ]
            row = row.strip('\r\n')
            print(row)
            if test_type == "fixed_rs":
                results[test_type][theta_s][r_c] = row
            else:
                results[test_type][theta_s][r_s] = row 

    with open(os.path.join(results_dir, "results_all.json"), "w") as f:
        json.dump(results, f)

    # Combine results
    print(results)
    for test_type in results.keys():
        a = results[test_type]
        with open(os.path.join(results_dir, "results_all_" + test_type + ".csv"), "w") as f:
            for theta_s in a.keys():
                f.write("theta_s = " + str(theta_s) + '\n')
                f.write(',beta_Aschner,beta_Tran,beta_Lam_LEF,n_total_omni,n_total_Aschner,n_total_Tran,n_total_Lam_LEF\n')
                b = a[theta_s]
                for r in sorted(b.keys()):
                    f.write(str(r) + ',' + b[r] + '\n')
                f.write('\n')
    

if __name__ == "__main__":
    timestamp = sys.argv[1]
    print("timestamp = ", timestamp)
    main(timestamp)