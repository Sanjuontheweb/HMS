// a simple program for hospital management system which can be seen in typical hospitals and Inspired by a Bank Management System
// This management app is for the staff members

/*
    created a struct for patient, in the main func, displays the meny.
    then another function for linking the option  tot the respective functions,
    admits a new patient and asks about most of their details, i have done some clever error handling anol hre. assigns a MRN & room no and saves it to a file
    discharges patients,
    checks up on patient details, medical equipment, blood supply, info on docs & nurses, books an appointment and cancel them

    And I've made getting the MRN number only once and never again, u just have to remember it
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

int func_selector(char choice);
int appointment();
int new_patient();
int discharge_patient();
int blood_supp();
int med_equipment();
int docs_nurses();
int patient_details();
int cancel_appointment();

//for handling patient related
typedef struct
{
    char MRN[50];
    int pat_no;
    char status[20];

    char name[50];
    char dob[20];
    char gender[20];  // 20 bytes for LGBTQ
    char blood_group[20];
    char contact_no[20];
    char emergency_contact[20];
    char diagnosed_condition[50];
    char COA[75];
    char TOA[50];

    char alloted_room_no[10];
} Patient;

typedef struct
{
    char university[50];
    int doc_id;
    char qualification[20];

    char name[50];
    char age[20];
    char gender[20];  // 20 bytes for LGBTQ
    char contact_no[20];
    char specialization[20];
    char experience[50]; 

    char alloted_patient[20];
    char TOA[50];   //time of appointment/hirement
    char status[50];     // appointed?fired?
} Doctor;

typedef struct
{
    char college[50];
    int nurse_id;
    char qualification[20];

    char name[50];
    char age[20];
    char contact_no[20];
    char TOA[50];   // time of appointment/hirement
    char status[50];   // appointed?fired?
} Nurses;

typedef struct
{
    int stretchers;
    int defibrillators;
    int anesthesia_machines;
    int pat_monitors;
    int sterilizers;
    int ECG;
    int surgical_tables;
    int blanket;
    int surgical_lights;
    int fluid_warmer;
} Medical_equipment;

//types of blood available
typedef struct
{
    int o_pos;
    int o_neg;
    int a_pos;
    int a_neg;
    int b_pos;
    int b_neg;
    int ab_pos;
    int ab_neg;
} Blood_supply;

#pragma pack(push, 1)
typedef struct {
    char booking_time[15];
    char doc_appoint[15];
    char name[20];
    char contact_no[20];
    int appoint_id;
} Appointment;
#pragma pack(pop)


// checks for the no of patients admitted relative to the hospital
int patient_no = 0;

// function to randomize MRN number and give it out
void MRN_nos(char MRNs[][50], int count) {    

    for (int i= 0;i<count;i++){
        // Generate a random 5-digit number between 10000 and 99999
        int random_number = rand() % 90000 + 10000;

        // Simulate assigning MRNs 
        sprintf(MRNs[i], "HMS%05d", random_number);
    }

}

// function to randomize doctor id
int doc_ids() {    
    return rand() % 101 + 110;
}

// function to randomize nurse id
int nurse_ids() {
    return rand() % 79 + 1;
}

int appoint_ids() {
    return rand() % 999999 + 100000;
}

// the hospital contains 50 rooms in total
void room_nos(char rooms[][18], int count) {
    for (int i = 0; i < count; i++) {
        // generate a random room no with room code
        int random_room = rand() % 90 + 10;
        char rm_code = 'A' + rand() % ('S' - 'A' + 1);

        sprintf(rooms[i], "%c%02d", rm_code, random_room);
    }
}

//current doctors at service
Doctor dokies[10] = {
        {"Harvard", 110, "MBBS", "Dr. John Smith", "35", "Male", "1234567890", "Cardiology", "10 years", "HMS41006", "sometime", "in service"},
        {"Stanford", 111, "MD", "Dr. Emily Davis", "42", "Female", "2345678901", "Neurology", "15 years", "HMS42463", "sometime", "in service"},
        {"Oxford", 112, "PhD", "Dr. Michael Brown", "50", "Male", "3456789012", "Pediatrics", "20 years", "HMS42258", "sometime", "Retired!"},
        {"MIT", 113, "MBBS", "Dr. Sarah Johnson", "30", "Female", "4567890123", "Oncology", "8 years", "HMS42026", "sometime", "in service"},
        {"Yale", 114, "MD", "Dr. James Miller", "40", "Male", "5678901234", "Orthopedics", "18 years", "HMS10104", "sometime", "Retired!"},
        {"Cambridge", 115, "PhD", "Dr. Olivia Wilson", "38", "Female", "6789012345", "Psychiatry", "12 years", "HMS10401", "sometime", "in service"},
        {"Princeton", 116, "MBBS", "Dr. Daniel Moore", "45", "Male", "7890123456", "Dermatology", "14 years", "HMS10626", "sometime", "in service"},
        {"Columbia", 117, "MD", "Dr. Sophia Taylor", "28", "Female", "8901234567", "Gynecology", "8 months", "HMS11005", "sometime", "Appointed!"},
        {"Harvard", 118, "PhD", "Dr. Benjamin Lee", "50", "Male", "9012345678", "Gastroenterology", "22 years", "HMS11237", "sometime", "in service"},
        {"Stanford", 119, "MBBS", "Dr. Ava Harris", "33", "Female", "0123456789", "Urology", "7 years", "HMS11452", "sometime", "in service"}
};

// current nurses at service
Nurses nurcie[6] = {
    {"Denver College", 82, "BSc Nursing", "Alice Johnson", "28", "1234567890", "sometime", "in service"},
    {"Florida State College", 83, "Diploma in Nursing", "Rachana Smith", "32", "2345678901", "sometime", "in service"},
    {"North carolina College", 84, "MSc Nursing", "Sunitha Williams", "30", "3456789012", "sometime", "in service"},
    {"Oxford University", 85, "Registered Nurse", "Chei Lee", "35", "4567890123", "sometime", "in service"},
    {"Georgia Southwest State College", 86, "BSc Nursing", "Emma Davis", "26", "5678901234", "sometime", "Appointed!"},
    {"Saint Anthony College", 87, "Diploma in Nursing", "Debra Miller", "40", "6789012345", "sometime", "in service"}
};

// pre-existing medical equipment
// just what u would expect from a new hospital
Medical_equipment med_equip = {
    .stretchers = 100,
    .defibrillators = 100,
    .anesthesia_machines = 100,
    .pat_monitors = 100,
    .sterilizers = 100,
    .ECG = 100,
    .surgical_tables = 100,
    .blanket = 100,
    .surgical_lights = 100,
    .fluid_warmer = 100
};

// pre-existing blood supply
Blood_supply bld_supply = {
    .o_pos = 150,
    .o_neg = 50,
    .b_pos = 500,
    .b_neg = 80,
    .a_pos = 175,
    .a_neg = 45,
    .ab_pos = 12,
    .ab_neg = 1
};


int main(void)
{
    char choice;

    while (1)
    {
    printf("\n                <HOSPITAL MANAGEMENT SYSTEM>\n");
    printf("\nA> Admit a new patient\nB> Discharge a patient\nC> Change the stock of blood supplies\nD> Change the stock of medical equipment\nE> Docs and nurses\nF> Get any patient details\nG> Book an appointment\nH> Cancel an appointment\nI> EXIT\n\n");
    printf(" Enter your choice: ");
    scanf(" %c", &choice);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    printf("\n");

    if (choice == 'I' || choice == 'i') {
        exit(1);
    } else 
    {
        func_selector(choice);
    }
    }

    return 0;
}

int func_selector(char choice)
{
    choice = toupper(choice);

    switch(choice)
    {
        case 'A' :
            new_patient();
            break;

        case 'B':
            discharge_patient();
            break;

        case 'C':
            blood_supp();
            break;

        case 'D':
            med_equipment();
            break;

        case 'E':
            docs_nurses();
            break;

        case 'F':
            patient_details();
            break;

        case 'G':
            appointment();
            break;

        case 'H':
            cancel_appointment();
            break;

        case 'I':
            exit(1);

        default:
            printf("Please enter an appropriate response\n");
    }
}

int new_patient()
{
    Patient pat;

    // file handling
    FILE *admit_file = fopen("patients.dat", "ab+");
    FILE *pat_history_file = fopen("patients_history.dat", "ab+");

    if (admit_file == NULL) {
        fprintf(stderr, "\nAdmit file not opening...!!");
    } else if (pat_history_file == NULL) {
        fprintf(stderr, "\nThe patient history file is not opening!!!");
    }

    //time of admit(TOA)
    time_t now;
    time(&now);  // get current time

    // convert to local time
    struct tm *local = localtime(&now);

    // format as string and copy into pat.TOA
    strcpy(pat.TOA, asctime(local));

    // optional: remove trailing newline from asctime()
    pat.TOA[strcspn(pat.TOA, "\n")] = '\0';  // strip newline if needed

    // asking for details
    printf("Enter the patient's name: ");
    fgets(pat.name, sizeof(pat.name), stdin);
    pat.name[strcspn(pat.name, "\n")] = '\0';  // null terminates the names

    printf("Enter the patient's date of birth(DD/MM/YYYY): ");
    fgets(pat.dob, sizeof(pat.dob), stdin);
    pat.dob[strcspn(pat.dob, "\n")] = '\0';

    printf("Enter the patient's gender: ");
    fgets(pat.gender, sizeof(pat.gender), stdin);
    pat.gender[strcspn(pat.gender, "\n")] = '\0';

    printf("Enter the patient's blood group: ");
    fgets(pat.blood_group, sizeof(pat.blood_group), stdin);
    pat.blood_group[strcspn(pat.blood_group, "\n")] = '\0';

    // error handling for contact no.
    int valid = 0;

    do {
        printf("Enter the patient's contact no: ");
        fgets(pat.contact_no, 13, stdin);

        // Clear input buffer, only if the given input is 13 characters above
        if (!strchr(pat.contact_no, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        pat.contact_no[strcspn(pat.contact_no, "\n")] = '\0';

        valid = 1;
        for (int i = 0; pat.contact_no[i] != '\0'; i++) {
            if (!isdigit((unsigned char)pat.contact_no[i])) {
                valid = 0; // not valid
                printf("Invalid input! Please enter a valid phone no.\n\n");
                break;
            }
        }

    } while (!valid);

    // error handling for emergency contact no.
    do {
        printf("Enter the patient's emergency contact numbers: ");
        fgets(pat.emergency_contact, 13, stdin);

        // Clear input buffer
        if (!strchr(pat.emergency_contact, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        pat.emergency_contact[strcspn(pat.emergency_contact, "\n")] = '\0';
        valid = 1;
        for (int i = 0; pat.emergency_contact[i] != '\0'; i++) {
            if (!isdigit((unsigned char)pat.emergency_contact[i])) {
                valid = 0; // not valid
                printf("Invalid input! Please enter a valid emergency phone no.\n\n");
                break;
            }
        }

    } while (!valid);

    // for checking if the patient is diagnosed woth other conditions
    char conditon_y_n[10];
    int valid_ans_for_condition = 0;

    while (!valid_ans_for_condition)
    {
        printf("Enter if the patient is diagnosed with any conditions(y/n): ");
        fgets(conditon_y_n, sizeof(conditon_y_n), stdin);
        conditon_y_n[strcspn(conditon_y_n, "\n")] = '\0';

        if (conditon_y_n[0] == 'y' || conditon_y_n[0] == 'Y')
        {
            printf("\nEnter the patient's diagnosed condition: ");
            fgets(pat.diagnosed_condition, sizeof(pat.diagnosed_condition), stdin);
            pat.diagnosed_condition[strcspn(pat.diagnosed_condition, "\n")] = '\0';

            break;
        }
        else if (conditon_y_n[0] == 'n' || conditon_y_n[0] == 'N')
        {
            strcpy(pat.diagnosed_condition, "None");
            printf("Good!\n");
            break;
        }
        else
        {
            printf("\nJust answer with y / n\n");
        }

    }

    printf("\nCause of admit of patient(injury): ");
    fgets(pat.COA, sizeof(pat.COA), stdin);
    pat.COA[strcspn(pat.COA, "\n")] = '\0';

    // MRN
    char MRNs[1][50]; 
    srand(time(NULL));     // Seed the random number generator with the current time

    MRN_nos(MRNs, 1);
    printf("\nThe patient's MRN (Medical Record Number) is : %s\n", MRNs[0]);
    strncpy(pat.MRN, MRNs[0], sizeof(pat.MRN) - 1);
    pat.MRN[sizeof(pat.MRN) - 1] = '\0';  // these two lines are for buffer overflow probleam

    // room allotment
    char roomS[1][18];
    srand(time(NULL));

    room_nos(roomS, 1);
    printf("The patient's alloted room is : %s\n", roomS[0]);
    strncpy(pat.alloted_room_no, roomS[0], sizeof(pat.alloted_room_no) - 1);
    pat.alloted_room_no[sizeof(pat.alloted_room_no) - 1] = '\0';

    //change the status
    strcpy(pat.status, "Admitted!");

    // increase the patient's no. and store it's value in struct & a file
    FILE *state_file = fopen("state.dat", "rb");
    if (state_file != NULL) {
        fread(&patient_no, sizeof(patient_no), 1, state_file);
        fclose(state_file);
    }

    patient_no++;
    pat.pat_no = patient_no;

    state_file = fopen("state.dat", "wb");
    if (state_file != NULL) {
        fwrite(&patient_no, sizeof(patient_no), 1, state_file);
        fclose(state_file);
    } else {
        fprintf(stderr, "state file may have been corrupted");
    }


    //file writing
    fwrite(&pat, sizeof(pat), 1, admit_file);
    fwrite(&pat, sizeof(pat), 1, pat_history_file);

    if (fclose(admit_file) == 0 && fclose(pat_history_file) == 0) {
        printf("\n%s was successfully admitted\n", pat.name);
        return 0;
    } else {
        fprintf(stderr, "There was an error admitting the patient! Sorry for the Inconvienience");
        return 1;
    }
}


int discharge_patient()
{
    Patient pat_disc;
    Patient pat_hist;
    FILE *file = fopen("patients.dat", "rb+");
    FILE *state = fopen("state.dat", "rb");    //later i'm editing it so only rb
    FILE *history_file = fopen("patients_history.dat", "rb");
    FILE *temp_hist = fopen("temp_history.dat", "wb");
    FILE *temp = fopen("temp.dat", "wb");      // temp file for keeping others

    if (file == NULL || temp == NULL) {
        fprintf(stderr, "\nThe files are not opening!!!");
        return 1;
    }
    if (!history_file || !temp_hist) {
        fprintf(stderr, "\nError opening history files!");
        return 1;
    }

    char TOD[30];
    char discharge_MRN[25];

    //time of discharge(TOD)
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    strcpy(TOD, asctime(local));

    // optional: remove trailing newline from asctime()
    TOD[strcspn(TOD, "\n")] = '\0';

    // asking for the patient to be discharged
    printf("Enter the MRN of the patient to be discharged: ");
    scanf("%s", discharge_MRN);

    // main logic
    int found = 0;

    while(fread(&pat_disc, sizeof(pat_disc), 1, file))
    {
        if(strcmp(pat_disc.MRN, discharge_MRN) == 0)
        {
            printf("\n%s was DISCHARGED from room %s at %s\n", pat_disc.name, pat_disc.alloted_room_no, TOD);
            found = 1;
            // here, it skips writing it to the new file
        } 
        else 
        {
            fwrite(&pat_disc, sizeof(pat_disc), 1, temp);
        }
    }

    //kinda the same, but instead of deleting the record in patients.dat, im updating the record with patients_history.dat
    // i know i could've integrated into the above loop only or could've made the code less redundant, but it's fine

    if (strcmp(pat_hist.MRN, discharge_MRN) == 0) {
        found = 1;
    }
    
    if (found) {
        while (fread(&pat_hist, sizeof(pat_hist), 1, history_file))
        {
            if (strcmp(pat_hist.MRN, discharge_MRN) == 0) {
                strcpy(pat_hist.status, "Discharged!");
            }
            fwrite(&pat_hist, sizeof(pat_hist), 1, temp_hist);
        }
    }

    fclose(file);
    fclose(temp); 
    fclose(history_file);
    fclose(temp_hist);

    remove("patients_history.dat");
    rename("temp_history.dat", "patients_history.dat");

    if (found) {
        remove("patients.dat");
        rename("temp.dat", "patients.dat");  // this changes the status to discharged and removes the patient from patients.dat
    } else {
        remove("temp.dat");  // no changes
        printf("\nThere is no patient with the MRN: %s\n", discharge_MRN);
    }

    if (state != NULL) {
        fread(&patient_no, sizeof(patient_no), 1, state);
        fclose(state);
    }

    patient_no--;

    state = fopen("state.dat", "wb");
    if (state != NULL) {
        fwrite(&patient_no, sizeof(patient_no), 1, state);
        fclose(state);
    } else {
        fprintf(stderr, "state file may have been corrupted");
    }

    return 0;
}

// a helper function for adding or removinf from  the stock of blood supply
int blood_helper(int bloody_choice) {
    char your_choice[10];
    int add_blood;
    int remove_blood;
    int valid_option = 0;

    FILE *bld_file = fopen("blood_supply.dat", "rb+");
    if (bld_file == NULL) {
        bld_file = fopen("blood_supply.dat", "wb+");
        return -1;
    }

    fread(&bld_supply, sizeof(bld_supply), 1, bld_file);
    int *blood_type = NULL;

    switch(bloody_choice) {
        case 1: blood_type = &bld_supply.o_pos; break;
        case 2: blood_type = &bld_supply.o_neg; break;
        case 3: blood_type = &bld_supply.a_pos; break;
        case 4: blood_type = &bld_supply.a_neg; break;
        case 5: blood_type = &bld_supply.b_pos; break;
        case 6: blood_type = &bld_supply.b_neg; break;
        case 7: blood_type = &bld_supply.ab_pos; break;
        case 8: blood_type = &bld_supply.ab_neg; break;
        default:
            fprintf(stderr, "\nThere is no such blood type");
            fclose(bld_file);
            return -1;
    }

    printf("\nBlood packets: %d", *blood_type);
    printf("\nIf u want to add blood packets, press + \nor if u want to remove press - ");

    do {
        printf("\nYour choice? ");
        scanf("%s", your_choice);

        valid_option = 1;
        if(strcmp(your_choice, "+") == 0) {
            printf("\nHow many packets would u like to add: ");
            scanf("%d", &add_blood);
            *blood_type += add_blood;
            break;
        }
        else if (strcmp(your_choice, "-") == 0){
            printf("\nHow many packets would u like to remove: ");
            scanf("%d", &remove_blood);
            
            if (remove_blood >= *blood_type) {
                printf("\nYou can't remove more than %d blood packets", *blood_type);
            } else {
                *blood_type -= remove_blood;
            }
        }
        else {
            printf("\nThe options are + or -.Choose between the only two options.\n");
            valid_option = 0;  //invalid option
        }
    } while (!valid_option);

    rewind(bld_file);
    fwrite(&bld_supply, sizeof(bld_supply), 1, bld_file);
    fclose(bld_file);

    return 0;
}

int blood_supp()
{
    char blood_entered[10];
    int blood_choice;
    int valid = 0;

    FILE *bld_file = fopen("blood_supply.dat", "rb"); // read mode

    if (bld_file != NULL) {
        fread(&bld_supply, sizeof(Blood_supply), 1, bld_file);
        fclose(bld_file);
    } else {
        // File doesn't exist, will use default initialized values
        printf("No existing data found. Using default blood supply.\n");
    }

    //asking for which blood type to deal with
    printf("Which blood type do u wanna add or use? \n");
    printf("\n1> O positive\n2> O negative\n3> A positive\n4> A negative\n5> B positive\n6> B negative\n7> AB positive\n8> AB negative\n");

    while (!valid) {
        printf("\nEnter the number corresponding to the blood type you would like to deal with: ");
        if (fgets(blood_entered, sizeof(blood_entered), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }
        blood_entered[strcspn(blood_entered, "\n")] = '\0';

        // Check if input is empty
        if (blood_entered[0] == '\0') {
            printf("Input cannot be empty. Try again.\n");
             continue;
        }

        // Check if all characters are digits
        valid = 1;
        for (int i = 0; blood_entered[i] != '\0'; i++) {
            if (!isdigit((unsigned char)blood_entered[i])) {
                valid = 0;
                printf("Invalid input! Please enter digits only.\n");
                break;
            }
        }

        if (!valid) continue;

        // Convert string to integer
        blood_choice = atoi(blood_entered);

        // Validate range
        if (blood_choice < 1 || blood_choice > 8) {
            printf("Invalid input! Please enter a number between 1-10.\n");
            valid = 0;
        }
    }

    switch(blood_choice)
    {
        case 1:
            blood_helper(blood_choice);
            break;

        case 2:
            blood_helper(blood_choice);
            break;

        case 3:
            blood_helper(blood_choice);
            break;

        case 4:
            blood_helper(blood_choice);
            break;

        case 5:
            blood_helper(blood_choice);
            break;

        case 6:
            blood_helper(blood_choice);
            break;

        case 7:
            blood_helper(blood_choice);
            break;

        case 8:
            blood_helper(blood_choice);
            break;

        default:
            printf("\nEnter an appropriate response");
            break;

    }

    
    // for checking the blood supply
    char check_blood[10];
    char check_choice_entered[10];
    int valid_check = 0;
    int valid_check_inside = 0;
    int check_blood_choice;

    bld_file = fopen("blood_supply.dat", "rb");
    fread(&bld_supply, sizeof(bld_supply), 1, bld_file);

    while (!valid_check) {
        printf("\nWould you like to know the number of blood packets available(y/n): ");
        scanf("%s", check_blood);

        for (int i = 0; check_blood[i] != '\0'; i++) {
            check_blood[i] = toupper((unsigned char)check_blood[i]);
        }

        valid_check = 1;

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        if (strcmp(check_blood, "Y") == 0)
        {
            printf("\nWhich blood type would you like to check? ");
            printf("\n1> O positive\n2> O negative\n3> A positive\n4> A negative\n5> B positive\n6> B negative\n7> AB positive\n8> AB negative\n");
             
            while (!valid_check_inside) {
                printf("\nEnter the number corresponding to the blood type you would like to check out: ");
                if (fgets(check_choice_entered, sizeof(check_choice_entered), stdin) == NULL) {
                    printf("Input error.\n");
                    continue;
                }
                check_choice_entered[strcspn(check_choice_entered, "\n")] = '\0';

                // Check if input is empty
                if (check_choice_entered[0] == '\0') {
                    printf("Input cannot be empty. Try again.\n");
                    continue;
                }

                // Check if all characters are digits
                valid_check_inside = 1;
                for (int i = 0; check_choice_entered[i] != '\0'; i++) {
                    if (!isdigit((unsigned char)check_choice_entered[i])) {
                        valid_check_inside = 0;
                        printf("Invalid input! Please enter digits only.\n");
                        break;
                    }
                }

                if (!valid_check_inside) continue;

                // Convert string to integer
                check_blood_choice = atoi(check_choice_entered);

                // Validate range
                if (check_blood_choice < 1 || check_blood_choice > 10) {
                    printf("Invalid input! Please enter a number between 1-10.\n");
                    valid_check_inside = 0;
                }
            }

            switch(check_blood_choice) {
                case 1:
                    printf("O positive: %d\n", bld_supply.o_pos);
                    break;

                case 2:
                    printf("O negative: %d\n", bld_supply.o_neg);
                    break;
                    
                case 3:
                    printf("A positive: %d\n", bld_supply.a_pos);
                    break;

                case 4:
                    printf("A negative: %d\n", bld_supply.a_neg);
                    break;

                case 5:
                    printf("B positive: %d\n", bld_supply.b_pos);
                    break;

                case 6:
                    printf("B negative: %d\n", bld_supply.b_neg);
                    break;

                case 7:
                    printf("AB positive: %d\n", bld_supply.ab_pos);
                    break;

                case 8:
                    printf("AB negative: %d\n", bld_supply.ab_neg);
                    break;

                default:
                    printf("\nEnter an appropriate response\n");
            }
            fclose(bld_file);
            break;

        } else if (strcmp(check_blood, "N") == 0) {
            break;

        }
        else {
            printf("Please enter y or n\n");
            valid_check = 0;
        }
    }

    return 0;
}

// a helper function for adding or removing the stock of medical equipment
int equipment_handler(int equip_choice) {
    char your_choice[10];
    int add_stuff;
    int remove_stuff;
    int valid_option = 0;

    FILE *med_file = fopen("medical_equip.dat", "rb+");
    if (med_file == NULL) {
        med_file = fopen("medical_equip.dat", "wb+");
        return -1;
    }

    fread(&med_equip, sizeof(med_equip), 1, med_file);
    int *med_equipment = NULL;

    switch(equip_choice) {
        case 1: med_equipment = &med_equip.stretchers; break;
        case 2: med_equipment = &med_equip.defibrillators; break;
        case 3: med_equipment = &med_equip.anesthesia_machines; break;
        case 4: med_equipment = &med_equip.pat_monitors; break;
        case 5: med_equipment = &med_equip.sterilizers; break;
        case 6: med_equipment = &med_equip.ECG; break;
        case 7: med_equipment = &med_equip.surgical_tables; break;
        case 8: med_equipment = &med_equip.blanket; break;
        case 9: med_equipment = &med_equip.surgical_lights; break;
        case 10: med_equipment = &med_equip.fluid_warmer; break;
        default:
            fprintf(stderr, "\nSelect the equipment given in the list please..");
            fclose(med_file);
            return -1;
    }

    printf("\nNo. of medical equipmet: %d", *med_equipment);
    printf("\nIf u want to add medical equipment, press + \nor if u want to remove press - ");

    do {
        printf("\nYour choice? ");
        scanf("%s", your_choice);

        valid_option = 1;
        if(strcmp(your_choice, "+") == 0) {
            printf("\nHow many would u like to add: ");
            scanf("%d", &add_stuff);
            *med_equipment += add_stuff;
            break;
        }
        else if (strcmp(your_choice, "-") == 0){
            printf("\nHow many would u like to remove: ");
            scanf("%d", &remove_stuff);
            
            if (remove_stuff >= *med_equipment) {
                printf("\nYou can't remove more than %d equipments", *med_equipment);
            } else {
                *med_equipment -= remove_stuff;
            }
        }
        else {
            printf("\nThe options are + or -.Choose between the only two options.\n");
            valid_option = 0;  //invalid option
        }
    } while (!valid_option);

    rewind(med_file);
    fwrite(&med_equip, sizeof(med_equip), 1, med_file);
    fclose(med_file);

    return 0;
}


int med_equipment()
{
    char choice_entered[10];
    int equip_choice;
    int valid = 0;

    FILE *med_file = fopen("medical_equip.dat", "rb"); // read mode

    if (med_file != NULL) {
        fread(&med_equip, sizeof(Medical_equipment), 1, med_file);
        fclose(med_file);
    } else {
        // File doesn't exist, will use default initialized values
        printf("No existing data found. Using default equipment values.\n");
    }

    //asking for which eq to deal with
    printf("\nWhich Medical Equipment do u wanna deal with? \n");
    printf("\n1> Stretchers\n2> Defibrillators\n3> Anesthesia Machines\n4> Patient Monitors\n5> Sterilizers\n6> EKG/ECG Machines\n7> Surgical Tables\n8> Blankets\n9> Surgical Lights\n10> Fluid Warmers\n");

    while (!valid) {
        printf("\nEnter the number corresponding to the medical equipment you would like to deal with: ");
        if (fgets(choice_entered, sizeof(choice_entered), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }
        choice_entered[strcspn(choice_entered, "\n")] = '\0';

        // Check if input is empty
        if (choice_entered[0] == '\0') {
            printf("Input cannot be empty. Try again.\n");
             continue;
        }

        // Check if all characters are digits
        valid = 1;
        for (int i = 0; choice_entered[i] != '\0'; i++) {
            if (!isdigit((unsigned char)choice_entered[i])) {
                valid = 0;
                printf("Invalid input! Please enter digits only.\n");
                break;
            }
        }

        if (!valid) continue;

        // Convert string to integer
        equip_choice = atoi(choice_entered);

        // Validate range
        if (equip_choice < 1 || equip_choice > 10) {
            printf("Invalid input! Please enter a number between 1-10.\n");
            valid = 0;
        }
    }

    // variables for the switch function
    char your_choice[10];
    int add_stuff;
    int remove_stuff;
    int valid_option = 0;

    med_file = fopen("medical_equip.dat", "wb");

    switch(equip_choice)
    {
        case 1:
            equipment_handler(equip_choice);
            break;

        case 2:
            equipment_handler(equip_choice);
            break;

        case 3:
            equipment_handler(equip_choice);
            break;

        case 4:
            equipment_handler(equip_choice);
            break;

        case 5:
            equipment_handler(equip_choice);
            break;

        case 6:
            equipment_handler(equip_choice);
            break;

        case 7:
            equipment_handler(equip_choice);
            break;

        case 8:
            equipment_handler(equip_choice);
            break;

        case 9:
            equipment_handler(equip_choice);
            break;

        case 10:
            equipment_handler(equip_choice);
            break;

        default:
            printf("\nEnter a number between 1-10. Thanking you");
            break;
    }

    // for checking the equipmets
    // idk what's going on anymore
    char check_equip[10];
    char check_choice_entered[10];
    int valid_check = 0;
    int valid_check_inside = 0;
    int check_equip_choice;

    med_file = fopen("medical_equip.dat", "rb");
    fread(&med_equip, sizeof(med_equip), 1, med_file);

    while (!valid_check) {
        printf("\nWould you like to know the number of equipments there are(y/n): ");
        scanf("%s", check_equip);

        for (int i = 0; check_equip[i] != '\0'; i++) {
            check_equip[i] = toupper((unsigned char)check_equip[i]);
        }

        valid_check = 1;

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        if (strcmp(check_equip, "Y") == 0)
        {
            printf("\nWhich equipment would you like to check? ");
             printf("\n1> Stretchers\n2> Defibrillators\n3> Anesthesia Machines\n4> Patient Monitors\n5> Sterilizers\n6> EKG/ECG Machines\n7> Surgical Tables\n8> Blankets\n9> Surgical Lights\n10> Fluid Warmers\n");
             
             while (!valid_check_inside) {
                printf("\nEnter the number corresponding to the medical equipment you would like to check out: ");
                if (fgets(check_choice_entered, sizeof(check_choice_entered), stdin) == NULL) {
                    printf("Input error.\n");
                    continue;
                }
                check_choice_entered[strcspn(check_choice_entered, "\n")] = '\0';

                // Check if input is empty
                if (check_choice_entered[0] == '\0') {
                    printf("Input cannot be empty. Try again.\n");
                    continue;
                }

                // Check if all characters are digits
                valid_check_inside = 1;
                for (int i = 0; check_choice_entered[i] != '\0'; i++) {
                    if (!isdigit((unsigned char)check_choice_entered[i])) {
                        valid_check_inside = 0;
                        printf("Invalid input! Please enter digits only.\n");
                        break;
                    }
                }

                if (!valid_check_inside) continue;

                // Convert string to integer
                check_equip_choice = atoi(check_choice_entered);

                // Validate range
                if (check_equip_choice < 1 || check_equip_choice > 10) {
                    printf("Invalid input! Please enter a number between 1-10.\n");
                    valid_check_inside = 0;
                }
            }

            switch(check_equip_choice) {
                case 1:
                    printf("Stretchers: %d\n", med_equip.stretchers);
                    break;

                case 2:
                    printf("Defibrillators: %d\n", med_equip.defibrillators);
                    break;
                    
                case 3:
                    printf("Anesthesia Machines: %d\n", med_equip.anesthesia_machines);
                    break;

                case 4:
                    printf("Patient Monitors: %d\n", med_equip.pat_monitors);
                    break;

                case 5:
                    printf("Sterilizers: %d\n", med_equip.sterilizers);
                    break;

                case 6:
                    printf("EKG/ECG Machines: %d\n", med_equip.ECG);
                    break;

                case 7:
                    printf("Surgical Tables: %d\n", med_equip.surgical_tables);
                    break;

                case 8:
                    printf("Blankets: %d\n", med_equip.blanket);
                    break;

                case 9:
                    printf("Surgical Lights: %d\n", med_equip.surgical_lights);
                    break;

                case 10:
                    printf("Fluid Warmers: %d\n", med_equip.fluid_warmer);
                    break;
            }
            fclose(med_file);
            break;

        } else if (strcmp(check_equip, "N") == 0) {
            break;

        }
        else {
            printf("Please enter y or n\n");
            valid_check = 0;
        }
    }

    return 0;
}

int doctor_check(char check_doc[10]) {
    printf("\n                      <Doctor Info>\n");
    printf("________________________________________________________\n\n");

    for (int i = 0; i<10; i++)
    {
        if (dokies[i].doc_id == atoi(check_doc)) {
            printf("%-30s %s\n", "Status: ", dokies[i].status);
            printf("\n");

            printf("%-30s %s\n", "Name: ", dokies[i].name);
            printf("%-30s %s\n", "Age: ", dokies[i].age);
            printf("%-30s %s\n", "Gender/Sex: ", dokies[i].gender);
            printf("%-30s %s\n", "Contact no: ", dokies[i].contact_no);
            printf("%-30s %s\n", "Specialization: ", dokies[i].specialization);
            printf("%-30s %s\n", "Experience: ", dokies[i].experience);
            printf("%-30s %s\n", "Time of Appointment: ", dokies[i].TOA);
            printf("%-30s %s\n", "Allotted Patient: ", dokies[i].alloted_patient);
            
            printf("%-30s %s\n", "University: ", dokies[i].university);
            printf("%-30s %s\n", "Qualification: ", dokies[i].qualification);
            break;    // exit once the doc_id matches the choice entered by the user
        }
    }

    printf("________________________________________________________\n");

    return 0;
}

int nurse_check(char check_nurse[10]) {
    printf("\n                        <Nurse Info>\n");
    printf("__________________________________________________________\n\n");

    for (int i = 0; i<6; i++)
    {
        if (nurcie[i].nurse_id == atoi(check_nurse)) {
            printf("%-30s %s\n", "Status: ", nurcie[i].status);
            printf("\n");

            printf("%-30s %s\n", "Name: ", nurcie[i].name);
            printf("%-30s %s\n", "Age: ", nurcie[i].age);
            printf("%-30s %s\n", "Contact no: ", nurcie[i].contact_no);
            printf("%-30s %s\n", "College: ", nurcie[i].college);
            printf("%-30s %s\n", "Qualification: ", nurcie[i].qualification);
            printf("%-30s %s\n", "Time of Appointment: ", nurcie[i].TOA);
            break;    // exit once the nurse_id matches the choice entered by the user
        }
    }

    printf("__________________________________________________________\n");

    return 0;
}

int docs_nurses()
{
    SetConsoleOutputCP(CP_UTF8);
    
    // for writing the existing 10 doctors in the hospital, it skips if they are already written
    FILE *doc_file;
    FILE *nurse_file;
    int doc_count = 0;
    int nurse_count = 0;

    doc_file = fopen("docs.dat", "rb+");
    if (doc_file == NULL) { // file doesn't exist
        doc_file = fopen("docs.dat", "wb");
        fwrite(dokies, sizeof(Doctor), 10, doc_file);
        doc_count = 10;
    } else {
        fseek(doc_file, 0, SEEK_END);
        doc_count = ftell(doc_file) / sizeof(Doctor);
    }
    fclose(doc_file);

    nurse_file = fopen("nurses.dat", "rb+");
    if (nurse_file == NULL) {
        nurse_file = fopen("nurses.dat", "wb");
        fwrite(nurcie, sizeof(Nurses), 6, nurse_file);
        nurse_count = 6;
    } else {
        fseek(nurse_file, 0, SEEK_END);
        nurse_count = ftell(nurse_file) / sizeof(Nurses);
    }
    fclose(nurse_file);


    // for appointing or firing doctos / nurses
    Doctor docks;
    Nurses nurcs;

    // red pill for firing a doctor and blue pill for appointing one
    char pill_doc[10];
    char pill_nurse[10];
    char desire[10];
    char doc_nurse[10];
    char doc_nurse_af[10];
    char check_doc[10];
    char check_nurse[10];

    int valid = 0;

    while(!valid)
    {
        valid = 1;

        printf("6> Would you like to check out a doctor or a nurse..");
        printf("\n9> Or would you like to do something to them😉 \n");
        fgets(desire, sizeof(desire), stdin);
        desire[strcspn(desire, "\n")] = '\0';

        if (strcmp(desire, "6") == 0) {
            doc_file = fopen("docs.dat", "rb");
            fread(dokies, sizeof(Doctor), doc_count, doc_file);
            nurse_file = fopen("nurses.dat", "rb");
            fread(nurcie, sizeof(Nurses), nurse_count, nurse_file);

            int check_out_valid = 0;
            do {
                check_out_valid = 1;
                
                printf("\nWould you like to check out D> a doctor OR \n");
                printf("                              N> a nurse\n");
                printf(": ");
                fgets(doc_nurse, sizeof(doc_nurse), stdin);
                doc_nurse[strcspn(doc_nurse, "\n")] = '\0';

                for (int i = 0; doc_nurse[i] != '\0'; i++) {
                    doc_nurse[i] = toupper(doc_nurse[i]);  // Convert the character to uppercase
                }

                if (strcmp(doc_nurse, "D") == 0) { 
                    int doc_check_valid = 0;

                    while (!doc_check_valid) {
                        doc_check_valid = 1;
                        printf("\nWhich doctor would u like to check out? ");

                        for (int i = 0; i < doc_count; i++) {
                            printf("\n%d> %s", dokies[i].doc_id, dokies[i].name);
                        }
                        printf("\n");
                        fgets(check_doc, sizeof(check_doc), stdin);
                        check_doc[strcspn(check_doc, "\n")] = '\0';
                        
                        if (atoi(check_doc) < 100 || atoi(check_doc) > 210) {
                            printf("\nInvalid input! Please enter a number between 100 & 210(doc_id).\n");
                            doc_check_valid = 0;
                        } else {
                            doctor_check(check_doc);
                            fclose(doc_file);
                            fclose(nurse_file);
                            break;
                        }
                    }
                }
                else if (strcmp(doc_nurse, "N") == 0)
                {
                    int nurse_check_valid = 0;

                    while (!nurse_check_valid) {
                        nurse_check_valid = 1;
                        printf("\nWhich nurse would u like to check out? ");

                        for (int i = 0; i < nurse_count; i++) {
                            printf("\n%d> %s", nurcie[i].nurse_id, nurcie[i].name);
                        }
                        printf("\n");
                        fgets(check_nurse, sizeof(check_nurse), stdin);
                        check_nurse[strcspn(check_nurse, "\n")] = '\0';

                        if (atoi(check_nurse) < 1 || atoi(check_nurse) > 90) {
                            printf("\nInvalid input! Please enter a number between 1 & 90(nurse_id).\n");
                            nurse_check_valid = 0;
                        } else {
                            nurse_check(check_nurse);
                            fclose(nurse_file);
                            fclose(doc_file);
                            break;
                        }
                    }
                    
                }
                else{
                    printf("\nENTER either D or N\n");
                    check_out_valid = 0;   // not valid, loop continues
                }
            } while(!check_out_valid);

        }
        else if (strcmp(desire, "9") == 0) {
            int valid_af = 0;

            while(!valid_af) {
                printf("\nWould you like to appoint/fire D> a doctor OR \n");
                printf("                               N> a nurse\n");
                printf("? ");

                fgets(doc_nurse_af, sizeof(doc_nurse_af), stdin);
                doc_nurse_af[strcspn(doc_nurse_af, "\n")] = '\0';

                for (int i = 0; doc_nurse_af[i] != '\0'; i++) {
                    doc_nurse_af[i] = toupper(doc_nurse_af[i]);  // Convert the character to uppercase
                }

                if (strcmp(doc_nurse_af, "D") == 0) {
                    // appointing or firing a doctor

                    doc_file = fopen("docs.dat", "rb+");
                    fread(dokies, sizeof(Doctor), doc_count, doc_file);

                    int doc_af_valid = 0;
                    printf("\nWould you like to appoint or fire a doctor?\n");
                    
                    while (!doc_af_valid) {
                        doc_af_valid = 1;

                        printf("B> Blue pill: Appoint a doc\n");
                        printf("R> Red pill: Fire a doc\n");
                        printf(": ");

                        fgets(pill_doc, sizeof(pill_doc), stdin);
                        pill_doc[strcspn(pill_doc, "\n")] = '\0';

                        for (int i = 0; pill_doc[i] != '\0'; i++) {
                            pill_doc[i] = toupper(pill_doc[i]);
                        }

                        if (strcmp(pill_doc, "B") == 0) {
                            //appoint a doc

                            //time of admit(TOA)
                            time_t now;
                            time(&now);  // get current time
                            struct tm *local = localtime(&now);
                            strcpy(docks.TOA, asctime(local));

                            // remove trailing newline from asctime()
                            docks.TOA[strcspn(docks.TOA, "\n")] = '\0';  // strip newline if needed

                            // asking for details
                            printf("Enter the doctor's name: ");
                            fgets(docks.name, sizeof(docks.name), stdin);
                            docks.name[strcspn(docks.name, "\n")] = '\0';  // null terminates the names

                            printf("Enter the doctor's age: ");
                            fgets(docks.age, sizeof(docks.age), stdin);
                            docks.age[strcspn(docks.age, "\n")] = '\0';

                            printf("Enter the doctor's gender: ");
                            fgets(docks.gender, sizeof(docks.gender), stdin);
                            docks.gender[strcspn(docks.gender, "\n")] = '\0';

                            int valid_doc_no = 0;
                            do {
                                printf("Enter the doctor's contact number: ");
                                fgets(docks.contact_no, 13, stdin);

                                // Clear input buffer
                                if (!strchr(docks.contact_no, '\n')) {
                                    int ch;
                                    while ((ch = getchar()) != '\n' && ch != EOF);
                                }

                                docks.contact_no[strcspn(docks.contact_no, "\n")] = '\0';
                                valid_doc_no = 1;
                                for (int i = 0; docks.contact_no[i] != '\0'; i++) {
                                    if (!isdigit((unsigned char)docks.contact_no[i])) {
                                        valid_doc_no = 0; // not valid
                                        printf("Invalid input! Please enter a valid phone no.\n\n");
                                        break;
                                    }
                                }

                            } while (!valid_doc_no);

                            printf("Enter the university attended for the qualification: ");
                            fgets(docks.university, sizeof(docks.university), stdin);
                            docks.university[strcspn(docks.university, "\n")] = '\0';

                            printf("Enter the doctor's qualification: ");
                            fgets(docks.qualification, sizeof(docks.qualification), stdin);
                            docks.qualification[strcspn(docks.qualification, "\n")] = '\0';

                            printf("Enter the doctor's specialization: ");
                            fgets(docks.specialization, sizeof(docks.specialization), stdin);
                            docks.specialization[strcspn(docks.specialization, "\n")] = '\0';

                            printf("Enter the doctor's experience in healing patients(time) & more info if possible: ");
                            fgets(docks.experience, sizeof(docks.experience), stdin);
                            docks.experience[strcspn(docks.experience, "\n")] = '\0';


                            // to assign the doctor a patient or not
                            char conditon_y_n[10];
                            int valid_ans_for_condition = 0;

                            while (!valid_ans_for_condition)
                            {
                                printf("Do you wanna assign a patient to the doctor(y/n), (you need the patient's MRN): ");
                                fgets(conditon_y_n, sizeof(conditon_y_n), stdin);
                                conditon_y_n[strcspn(conditon_y_n, "\n")] = '\0';

                                if (conditon_y_n[0] == 'y' || conditon_y_n[0] == 'Y')
                                {
                                    printf("\nEnter the patient's MRN: ");
                                    fgets(docks.alloted_patient, sizeof(docks.alloted_patient), stdin);
                                    docks.alloted_patient[strcspn(docks.alloted_patient, "\n")] = '\0';

                                    break;
                                }
                                else if (conditon_y_n[0] == 'n' || conditon_y_n[0] == 'N')
                                {
                                    strcpy(docks.alloted_patient, "None");
                                    printf("None!\n");
                                    break;
                                }
                                else
                                {
                                    printf("\nJust answer with y / n\n");
                                }

                            }
                            // doc_id
                            srand(time(NULL));
                            docks.doc_id = doc_ids();
                            printf("\nThe doctor's ID is : %d\n", docks.doc_id);

                            //change the status
                            strcpy(docks.status, "Appointed!");

                            dokies[doc_count++] = docks;
                            rewind(doc_file);
                            fwrite(&docks, sizeof(Doctor), 1, doc_file);

                            printf("\n%s was appointed successfully\n", docks.name);

                            if (fclose(doc_file) == 0) {
                            valid_af = 1;
                            }
                            break;

                        }
                        else if (strcmp(pill_doc, "R") == 0) {
                            // fire

                            char doc_to_fire[10];
                            int doc_found = 0;
                            int doc_firing_no;
                            int doc_fire_valid = 0;

                            while (!doc_fire_valid) {
                                doc_file = fopen("docs.dat", "rb+");
                                if (!doc_file) {
                                    fprintf(stderr, "\nError opening doc file\n");
                                    break;
                                }
                                printf("\nWhich doctor do you wanna fire? (doc id please): ");
                                fgets(doc_to_fire, sizeof(doc_to_fire), stdin);
                                doc_to_fire[strcspn(doc_to_fire, "\n")] = '\0';

                                doc_firing_no = atoi(doc_to_fire);

                                Doctor temp;
                                int doctor_found = 0;

                                while (fread(&temp, sizeof(Doctor), 1, doc_file) == 1) {
                                    if (temp.doc_id == doc_firing_no) {
                                        strcpy(temp.status, "Fired!");
                                        fseek(doc_file, -sizeof(Doctor), SEEK_CUR);
                                        fwrite(&temp, sizeof(Doctor), 1, doc_file);
                                        printf("\n%s was fired from the hospital!\n", temp.name);
                                        doctor_found = 1;
                                        doc_fire_valid = 1; // Exit outer loop
                                        break;
                                    }
                                }

                                if (!doctor_found) {
                                    fprintf(stderr, "\nNo doctor has a doc-id %d.\n", doc_firing_no);
                                }

                               if (fclose(doc_file) == 0) {
                                    valid_af = 1;
                               }
                            }
                        }
                        else {
                            fprintf(stderr, "\nTake the red pill or the blue pill. You have to choose one\n");
                            doc_af_valid = 0;
                        }
                    }

                }
                else if (strcmp(doc_nurse_af, "N") == 0) {
                    // appointing or firing a nurse

                    int nurse_af_valid = 0;
                    printf("\nWould you like to appoint or fire a nurse?\n");
                    
                    while (!nurse_af_valid) {
                        nurse_af_valid = 1;
                    
                        printf("B> Blue pill: Appoint a nurse\n");
                        printf("R> Red pill: Fire a nurse\n");
                        printf(": ");

                        fgets(pill_nurse, sizeof(pill_nurse), stdin);
                        pill_nurse[strcspn(pill_nurse, "\n")] = '\0';

                        for (int i = 0; pill_nurse[i] != '\0'; i++) {
                            pill_nurse[i] = toupper(pill_nurse[i]);
                        }

                        if (strcmp(pill_nurse, "B") == 0) {
                            //appoint a nurce
                            nurse_file = fopen("nurses.dat", "ab");
                            fread(nurcie, sizeof(Nurses), nurse_count, nurse_file);

                            //time of admit(TOA)
                            time_t now;
                            time(&now);  // get current time
                            struct tm *local = localtime(&now);
                            strcpy(nurcs.TOA, asctime(local));

                            // remove trailing newline from asctime()
                            nurcs.TOA[strcspn(nurcs.TOA, "\n")] = '\0';  // strip newline if needed

                            // asking for details
                            printf("Enter the nurse's name: ");
                            fgets(nurcs.name, sizeof(nurcs.name), stdin);
                            nurcs.name[strcspn(nurcs.name, "\n")] = '\0';  // null terminates the names

                            printf("Enter the nurse's age: ");
                            fgets(nurcs.age, sizeof(nurcs.age), stdin);
                            nurcs.age[strcspn(nurcs.age, "\n")] = '\0';

                            int valid_nurse_no = 0;
                            do {
                                printf("Enter the nurse's contact number: ");
                                fgets(nurcs.contact_no, 13, stdin);

                                if (!strchr(nurcs.contact_no, '\n')) {
                                    int ch;
                                    while ((ch = getchar()) != '\n' && ch != EOF);
                                }

                                nurcs.contact_no[strcspn(nurcs.contact_no, "\n")] = '\0';
                                valid_nurse_no = 1;
                                for (int i = 0; nurcs.contact_no[i] != '\0'; i++) {
                                    if (!isdigit((unsigned char)nurcs.contact_no[i])) {
                                        valid_nurse_no = 0; // not valid
                                        printf("Invalid input! Please enter a valid phone no.\n\n");
                                        break;
                                    }
                                }

                            } while (!valid_nurse_no);

                            printf("Enter the college attended for the qualification: ");
                            fgets(nurcs.college, sizeof(nurcs.college), stdin);
                            nurcs.college[strcspn(nurcs.college, "\n")] = '\0';

                            printf("Enter the nurse's qualification: ");
                            fgets(nurcs.qualification, sizeof(nurcs.qualification), stdin);
                            nurcs.qualification[strcspn(nurcs.qualification, "\n")] = '\0';

                            // nurse_id
                            srand(time(NULL));
                            nurcs.nurse_id = nurse_ids();
                            printf("\nThe nurse's ID is : %d\n", nurcs.nurse_id);

                            //change the status
                            strcpy(nurcs.status, "Appointed!");
                            nurcie[nurse_count++] = nurcs;

                            rewind(nurse_file);
                            fwrite(&nurcs, sizeof(Nurses), 1, nurse_file);

                            printf("\n%s was appointed successfully\n", nurcs.name);
                            if (fclose(nurse_file) == 0) {
                                valid_af = 1;
                            }
                            
                            break;

                        }
                        else if (strcmp(pill_nurse, "R") == 0) {
                            // fire

                            char nurse_to_fire[10];
                            int nurse_found = 0;
                            int nurse_firing_no;
                            int nurse_fire_valid = 0;

                            while (!nurse_fire_valid) {
                                nurse_file = fopen("nurses.dat", "rb+");
                                if (!nurse_file) {
                                    fprintf(stderr, "\nError opening nurses.dat\n");
                                    break;
                                }
                                printf("\nWhich nurse do you wanna fire? (nurse id please): ");
                                fgets(nurse_to_fire, sizeof(nurse_to_fire), stdin);
                                nurse_to_fire[strcspn(nurse_to_fire, "\n")] = '\0';

                                nurse_firing_no = atoi(nurse_to_fire);

                                Nurses temp;
                                int nurse_found = 0;

                                while (fread(&temp, sizeof(Nurses), 1, nurse_file) == 1) {
                                    if (temp.nurse_id == nurse_firing_no) {
                                        strcpy(temp.status, "Fired!");
                                        fseek(nurse_file, -sizeof(Nurses), SEEK_CUR);
                                        fwrite(&temp, sizeof(Nurses), 1, nurse_file);
                                        printf("\n%s was fired from the hospital!\n", temp.name);
                                        nurse_found = 1;
                                        nurse_fire_valid = 1; // Exit outer loop
                                        break;
                                    }
                                }

                                if (!nurse_found) {
                                    fprintf(stderr, "\nNo nurse has a nurse-id %d.\n", nurse_firing_no);
                                }

                               if (fclose(nurse_file) == 0) {
                                    valid_af = 1;
                               }
                            }
                        }
                        else {
                            fprintf(stderr, "\nTake the red pill or the blue pill. You have to choose one\n");
                            nurse_af_valid = 0;
                        }

                    break;
                    }
                }
                else {
                    fprintf(stderr, "\nChoose between D & N\n");
                    valid_af = 0;
                }
            }
        }
        else {
            printf("\nEnter an correct choice 6 or 9?\n");
            valid = 0;
        }
        
    }

    return 0;
}

// getting the info from history file as it can still get the patient's details even after dischargement.
// but, i have not given the option to edit any info, once given no changing
const char *HISTORY_FILE = "patients_history.dat"; 
int patient_details()
{
    Patient pat_read;
    FILE *file =  fopen(HISTORY_FILE, "rb");

    if (file == NULL) {
        fprintf(stderr, "\nThe patient history file is not opening!!!");
        return 1;
    }

    char find_MRN[50];
    printf("Enter the patient's MRN: ");
    scanf("%s", find_MRN);

    while(fread(&pat_read, sizeof(pat_read), 1, file))
    {
        if (strcmp(pat_read.MRN, find_MRN) == 0)
        {
            printf("---------------------------------------------------------------------------------------");
            printf("\n                        PATIENT'S TEMPORARY DETAILS REPORT\n");
            printf("---------------------------------------------------------------------------------------\n\n");
            
            printf("%-30s %s\n", "Status: ", pat_read.status);
            printf("\n");

            printf("%-30s %s\n", "Name: ", pat_read.name);
            printf("%-30s %s\n", "Date of Birth: ", pat_read.dob);
            printf("%-30s %s\n", "Gender/Sex: ", pat_read.gender);
            printf("%-30s %s\n", "Blood Group: ", pat_read.blood_group);
            printf("%-30s %s\n", "Contact No: ", pat_read.contact_no);
            printf("%-30s %s\n", "Emergency Contact No: ", pat_read.emergency_contact);

            printf("%-30s %s\n", "Pre-existing Conditions: ", pat_read.diagnosed_condition);
            printf("%-30s %s\n", "Cause of admit: ", pat_read.COA);
            printf("%-30s %s\n", "Time of admit: ", pat_read.TOA);

            printf("%-30s %s\n", "Allocated room number: ", pat_read.alloted_room_no);
            printf("\n---------------------------------------------------------------------------------------\n");
            fclose(file);
            return 0;
        }
    }
    fclose(file);

    printf("Patient with %s was not found!!\n", find_MRN);
    return 0;
}

int appointment() {
    Appointment app;

    FILE *app_file = fopen("appointments.dat", "ab");
    if (!app_file) {
        perror("Error opening file");
        exit(1);
    }

    FILE *doc_file = fopen("docs.dat", "rb");
    if (doc_file) {
        int doc_count = 0;
        while (fread(&dokies[doc_count], sizeof(Doctor), 1, doc_file) == 1) {
        doc_count++;
    }
    fclose(doc_file);

    printf("Your name(Kimi No Na wa)> ");
    fgets(app.name, sizeof(app.name), stdin);
    app.name[strcspn(app.name, "\n")] = '\0';

    int valid_no = 0;
    do {
        printf("Enter your contact details> ");
        fgets(app.contact_no, 13, stdin);

        // Clear input buffer
        if (!strchr(app.contact_no, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        app.contact_no[strcspn(app.contact_no, "\n")] = '\0';
        valid_no = 1;
        for (int i = 0; app.contact_no[i] != '\0'; i++) {
            if (!isdigit((unsigned char)app.contact_no[i])) {
                valid_no = 0; // not valid
                printf("Invalid input! Please enter a valid phone no.\n\n");
                break;
            }
        }

    } while (!valid_no);

    printf("\nWhat time would you like to book an appointment> ");
    fgets(app.booking_time, sizeof(app.booking_time), stdin);
    app.booking_time[strcspn(app.booking_time, "\n")] = '\0';

    int valid = 0;
    do {    
        valid = 1;

        printf("\nWhich doctor would you prefer for booking an appointment? ");
        for (int i = 0; i < doc_count; i++) {
            printf("\n%d> %s", dokies[i].doc_id, dokies[i].name);
        }
        printf("\n");
        fgets(app.doc_appoint, sizeof(app.doc_appoint), stdin);
        app.doc_appoint[strcspn(app.doc_appoint, "\n")] = '\0';
        
        if (atoi(app.doc_appoint) < 100 || atoi(app.doc_appoint) > 210) {
            printf("\nInvalid input! Please enter a number between 100 & 210(doc_id).\n");
            valid = 0;
        } else {
            srand(time(NULL));
            app.appoint_id = appoint_ids();

            printf("\nAn appointment was booked succesfully!\n");
            printf("Appointment ID: %d\n", app.appoint_id);
            fwrite(&app, sizeof(Appointment), 1, app_file);
            fclose(app_file);
            break;
        }

    } while (!valid);
    }

    return 0;
}

int cancel_appointment() {
    Appointment app_cancel;
    int given_appoint_id;
    int found = 0;

    FILE *app_file = fopen("appointments.dat", "rb+");
    FILE *temp = fopen("temp_app.dat", "wb");
    if (!app_file || !temp) {
        perror("Error opening file");
        return 1;
    }

    printf("Appointment ID: ");
    scanf("%d", &given_appoint_id);

    while (fread(&app_cancel, sizeof(Appointment), 1, app_file) == 1) {

        if (app_cancel.appoint_id == given_appoint_id) {
            printf("\nThe appointment has been cancelled!\n");
            found = 1;
                // here, it skips writing it to the new file
            } 
            else 
            {
                fwrite(&app_cancel, sizeof(app_cancel), 1, temp);
            }
        }

        fclose(app_file);
        fclose(temp);

        if (found) {
            remove("appointments.dat");
            rename("temp_app.dat", "appointments.dat");  // this changes the status to discharged and removes the patient from patients.dat
        } else {
            remove("temp_app.dat");  // no changes
            printf("\nThere is no appointment with ID: %d\n", given_appoint_id);
        }

        return 0;
    }

/*
                                                                        What i learnt from this project

    error handling is shit in C

    when i put a program inside a if (condition), the program runs automatically, don't know yyyy
    apparently to evaluate the condition or something..

    file handling is somewhat easy
    and somewhat compilcated

    to get the correct output from user, you have to keep it in loop

*/