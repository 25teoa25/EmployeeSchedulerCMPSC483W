import pandas as pd
import random

def generate_sorted_nurse_schedule_no_index_csv(departments, nurse_types, shifts=42, min_nurses=1, max_nurses=60, output_file="NurseContraints.csv"):
    """
    Generates a CSV file with random nurse requirements for each shift across multiple departments, sorted by shift, without the index column.
    
    Parameters:
        departments (list of str): List of department names.
        nurse_types (list of str): List of nurse type column headers.
        shifts (int): Number of shifts (default is 42).
        min_nurses (int): Minimum number of nurses required per shift (default is 1).
        max_nurses (int): Maximum number of nurses required per shift (default is 60).
        output_file (str): Name of the output CSV file.
    """
    # Generate data for all departments
    data = []
    for shift in range(1, shifts + 1):
        for department_name in departments:
            row = [department_name, shift]
            row.extend(random.randint(min_nurses, max_nurses) for _ in nurse_types)
            data.append(row)

    # Create DataFrame
    columns = ["Department", "Shift (1 - 42)"] + nurse_types
    df = pd.DataFrame(data, columns=columns)

    # Sort by shift
    df.sort_values(by="Shift (1 - 42)", inplace=True)

    # Save to CSV without the index
    df.to_csv(output_file, index=False)
    return df

# Define departments and nurse types
#departments = ["Oncology", "Pediatric", "Surgery,"]
departments = ['Oncology', 'Surgery', 'Pediatric', 'Family', 'Labor', 'Forensic', 'Critical Care', 'Mental Health', 'Midwife']
nurse_types = [
    "Nurse Type: Registered Nurse",
    "Nurse Type: Licensed Practical Nurse",
    "Nurse Type: Emergency Nursing"
]

# Generate the nurse schedule
sorted_no_index_df = generate_sorted_nurse_schedule_no_index_csv(departments, nurse_types)
