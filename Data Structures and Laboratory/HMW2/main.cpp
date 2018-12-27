/* 
 * File:   Second Homework of Data Structures
 * Author: Ozan Özyeğen
 *
 * Created on October 11, 2014, 4:33 PM
 */
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

struct nucleotide{
    string base;
    nucleotide* next;
    
};

nucleotide* initRna(string link){
    nucleotide* head; //Holds the head of the RNA
    nucleotide* traverse;
    nucleotide* temp;
    
    head = new nucleotide;
    traverse = head;
    //Creating the rna
    for(int i=0;i<10;i++){
        traverse->base = link[i];
        temp = new nucleotide;
        //It shouldnt work on the 10th time since the last node does not have a link to another node
        if(i!=9){
            traverse->next = temp;
            traverse = temp;
        }
    }
    //Making the last node's next NULL in order to prevent possible segmentation faults
    traverse->next = NULL;

    return head;
}

void printRnas(nucleotide* rna1,nucleotide* rna2,nucleotide* rna3,nucleotide* rna4,nucleotide* rna5){
    nucleotide* temp;
    for(int j=0;j<5;j++){
        if(j==0){
            cout << "1: ";
            temp = rna1;
        }else if(j==1){
            cout << "2: ";
            temp = rna2;
        }else if(j==2){
            cout << "3: ";
            temp = rna3;
        }else if(j==3){
            cout << "4: ";
            temp = rna4;
        }else if(j==4){
            cout << "5: ";
            temp = rna5;
        }

        for(int i=0;i<10;i++){
            cout << temp->base;
            if(i!=9)
                temp = temp->next;
        }
        cout << endl;
    }
}

bool IsCouple(string a,string b){
    //If both the rna nodes match function returns true otherwise it returns false
    if((a == "A" && b== "T")||(a == "T" && b== "A")||(a == "G" && b=="C")||(a == "C" && b=="G"))
        return true;
    return false;
}

string makeDna(string a){
    if(a == "A")
        return "AT";
    else if(a == "T")
        return "TA";
    else if(a == "G")
        return "GC";
    else if(a == "C")
        return "CG";
}

nucleotide* coupleDna(nucleotide* rna1,nucleotide* rna2,nucleotide* rna3,nucleotide* rna4,nucleotide* rna5){
    //Putting dna into an array
    nucleotide **rna_list = new nucleotide*[5];
    rna_list[0] = rna1;
    rna_list[1] = rna2;
    rna_list[2] = rna3;
    rna_list[3] = rna4;
    rna_list[4] = rna5;

    
    int i = 0, j = 0;
    nucleotide *head; //Head of the first rna
    nucleotide *head2; //Head of the second rna
    nucleotide *head_dna; //Head of the dna
    nucleotide *dna = new nucleotide;
    nucleotide *temp;
    
    while(i<=3){
        j = i+1;
        head = rna_list[i];
        while(j<=4){
            head2=rna_list[j];
            for(int k=0;k<10;k++){
                if(!IsCouple(rna_list[i]->base,rna_list[j]->base)){
                    break;
                }
                if(k!=9){
                    rna_list[i]=rna_list[i]->next;
                    rna_list[j]=rna_list[j]->next;
                }else{
                    //Creating dna
                    head_dna = dna;
                    nucleotide *head3=head;
                    for (int a=0;a<10;a++){
                        dna->base = makeDna(head->base);
                        temp = new nucleotide;
                        if(a!=9)
                            dna->next = temp;
                        else{
                            dna->next = NULL;
                        }
                        dna = temp;
                        if(a!=9)
                            head=head->next;
                    }
                    // Deletion of RNA pointers
                    head=head3;
                    rna_list[i]=head;
                    rna_list[j]=head2;
                    for(int c=0;c<5;c++){
                        temp = rna_list[c];
                        for(int d=0;d<10;d++){
                            if(d!=9)
                                rna_list[c] = rna_list[c]->next;
                            delete temp;
                            temp = rna_list[c];
                        }
                    }
                     
                    return head_dna;
                    
                }
                
            }
            rna_list[i]=head;
            rna_list[j]=head2;
            j++;
        }
        i++;
    }
    
}

void printDna(nucleotide* dna){
     nucleotide *temp;
     temp = dna;
     cout << endl;
     for(int i=0;i<10;i++){
            cout << dna->base << " ";
            if(i!=9)
                dna = dna->next;
        }
        cout << endl;
        
        //Deletion sequence of DNA
        
        dna = temp;
        for(int i=0;i<10;i++){
            if(i!=9)
                dna = dna->next;
            delete temp;
            temp = dna;
        }
         
        
}

int main(int argc, char** argv) {
    
    nucleotide *rna1 = initRna("ATGATGATGC");
    nucleotide *rna2 = initRna("TCGCGCTAGC");
    nucleotide *rna3 = initRna("CGTCGTAAAC");
    nucleotide *rna4 = initRna("TATTTACGAA"); 
    nucleotide *rna5 = initRna("TACTACTACG"); 
    
    printRnas(rna1, rna2, rna3, rna4, rna5);

    nucleotide *dna = coupleDna(rna1, rna2, rna3, rna4, rna5); 
    printDna(dna); 
     

    
    return 0;
}

