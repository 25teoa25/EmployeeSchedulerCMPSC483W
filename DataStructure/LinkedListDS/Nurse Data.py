import numpy as np
import pandas as pd
import matplotlib.pyplot as pythPlt
import seaborn as sbn
import random


names = pd.read_csv('baby-names.csv') #https://github.com/hadley/data-baby-names/blob/master/baby-names.csv where the data was taken from.
names = names.drop('percent', axis=1)
names = names.drop('sex', axis=1)
names = names.drop('year', axis=1)

def generate_name_combinations(df, total_combinations=1000):
    first_names = df['name'].tolist()  # Convert 'name' column to a list
    name_combinations = []
    
    for _ in range(total_combinations):
        first_name = random.choice(first_names)  # Random first name
        last_name = random.choice(first_names)   # Random last name
        name_combinations.append(f"{first_name} {last_name}")
    
    return name_combinations



random_names = generate_name_combinations(names)

nurse_types = ['NA', 'RN', 'LPN']
nurse_department = ['Oncology', 'Surgery', 'Pediatric', 'Family', 'Labor', 'Forensic', 'Critical Care', 'Mental Health', 'Midwife']
weekdays = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'] * 2


df_random_names = pd.DataFrame(random_names, columns=['Full Name'])
df_random_names['Nurse Number'] = range(1, len(df_random_names) + 1)  # Assign numbers from 1 to the total number of nurses
df_random_names['Nurse Type'] = [random.choice(nurse_types) for _ in range(len(df_random_names))]
df_random_names['Nurse Department'] = [random.choice(nurse_department) for _ in range(len(df_random_names))]

for i in range(1, 43):
    df_random_names[str(i)] = [random.choice([0, 1, 2]) for _ in range(len(df_random_names))]


df_random_names.to_csv('Nurse_List_Department_Included', index=False)
