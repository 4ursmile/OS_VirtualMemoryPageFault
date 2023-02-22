#include <iostream>
#include <limits>
using namespace std;
int n = 11;
int nOpf = 3;
int NewArr[100] = {2,1,5,2,1,1,0,9,0,0,7};
int PageFrames[100][100];
//This array to log the time page access to pageframes
int PageFrameExtend[100];
//This array to sign page fault symbol, use for print.
char PageFaultRowExtend[100];
int PageFaultCount = 0;
int FIFOPointer = 0;
// This function return true if a page are in pageframe and update extend(for LRU algorithm)
// or return false otherwise
bool checkPageInPageFrame(int pos){
    for (size_t i = 0; i < nOpf; i++){
        if (PageFrames[i][pos] == NewArr[pos]){
        PageFrameExtend[i] = pos;
        return true;
        }
    }
    return false;
}
//This function-just for LRU algorithm return true and push page to void space
//and update extend if there a void space
//or return false otherwise
bool IsHasVoidSpaceLRU(int pos){
    for (int j = 0; j < nOpf; j++){
        if (PageFrames[j][pos] == -1){
            PageFrames[j][pos] = NewArr[pos];
            PageFrameExtend[j] = pos;
            return true;
        }
    }
    return false;
}
//This function return true and push page to void space if there a void space
// return false otherwise
bool IsHasVoidSpace(int pos){
    for (int j = 0; j < nOpf; j++){
        if (PageFrames[j][pos] == -1){
            PageFrames[j][pos] = NewArr[pos];
            return true;
        }
    }
    return false;
}
//this funcion wil choose a victim to swap out and swap in new page use LRU algorithm
void SwapVictimPageLRU(int pos){
    int victimPos = 0;
    int earlyIn = PageFrameExtend[0];
    //find page earliest in pageframe
    for (int i = 0; i < nOpf; i++){
        if (PageFrameExtend[i] < earlyIn){
            victimPos = i;
            earlyIn = PageFrameExtend[i];
        }
    }
    PageFrames[victimPos][pos] = NewArr[pos];
    PageFrameExtend[victimPos] = pos;
}
//This funcion to copy current state of page frame to new state of pageframe
void CopytoNewPos(int pos){
for (size_t i = 0; i < nOpf; i++)
PageFrames[i][pos] = PageFrames[i][pos-1];
}
void LRUAlgorithmStart(){
    for (size_t i = 0; i < n; i++){
        //If not start page, copy from old state of page frame to new state of page frame
        if (i > 0)
        CopytoNewPos(i);
        //If page is in frame, just continnue
        if (checkPageInPageFrame(i)){
            PageFaultRowExtend[i] = ' ';
        //otherwise
        } else {
            PageFaultRowExtend[i] = '*';
            PageFaultCount++;
            //if there a void space in page frame, push coming page to it
            if (!IsHasVoidSpaceLRU(i))
            //otherwise choose a victim to swap out and swap in coming page
            SwapVictimPageLRU(i);
        }
    }
}
//this funcion wil choose a victim to swap out and swap in new page use FIFO algorithm
void SwapVictimPageFIFO(int pos){
    //pointer start from zero(first)
    PageFrames[FIFOPointer][pos] = NewArr[pos];
    //when swap pointer wil be update to next pageframe (page comes secondly)
    FIFOPointer++;
    //cyclic pointer in [0,n-1]
    if (FIFOPointer >= nOpf) FIFOPointer = 0;
}
void FIFOAlgorithmStart(){
    for (size_t i = 0; i < n; i++){
        if (i > 0)
            CopytoNewPos(i);
        if (checkPageInPageFrame(i)){
            PageFaultRowExtend[i] = ' ';
        } else {
            PageFaultRowExtend[i] = '*';
            PageFaultCount++;
            if (!IsHasVoidSpace(i))
                SwapVictimPageFIFO(i);
        }
    }
}
//this funcion wil choose a victim to swap out and swap in new page use OTP algorithm
void SwapVictimPageOTP(int pos){
    int remainCount = nOpf;
    int Map[nOpf] {0};
    // find first appear in future of pageframe in access string from page inpageframes
    for (size_t i = pos+1; i < n; i++){
        for (int j = 0; j < nOpf; j++){
            if (NewArr[i] == PageFrames[j][pos] && Map[j] == 0){
                remainCount--;
                Map[j] = i;
            }
        }
        if (remainCount == 0) break;
    }
    int maxValue = Map[0];
    int lastPos = 0;
    // from page find out above, choose page appear latest or not appear to swap out
    for (size_t i = 1; i < nOpf; i++){
        if (Map[i] > maxValue){
            maxValue = Map[i];
            lastPos = i;
        }else if (Map[i] == 0){
            lastPos = i;
            break;
        }
    }
    // swap in coming page.
    PageFrames[lastPos][pos] = NewArr[pos];
}
void OTPAlgorithmStart()
{
    for (size_t i = 0; i < n; i++){
        if (i > 0)
            CopytoNewPos(i);
        if (checkPageInPageFrame(i)){
            PageFaultRowExtend[i] = ' ';
        } else {
            PageFaultRowExtend[i] = '*';
            PageFaultCount++;
            if (!IsHasVoidSpace(i))
                SwapVictimPageOTP(i);
        }
    }
}
void init()
{
    for (size_t i = 0; i < nOpf; i++){
        for (size_t j = 0; j < n; j++)
            PageFrames[i][j] = -1;
    }
    for (size_t j = 0; j < nOpf; j++)
        PageFrameExtend[j] = INT_MAX;
}
int main(){
    int option1 = 1, option2 = 1;
    cout <<"--- Page Replacement algorithm ---\n";
    cout <<"1. Default referenced sequence\n";
    printf("2. Manual input sequence\n");
    cin >> option1;
    if (option1 != 1){
        cout <<"--Please enter number of reference string:";
        cin >> n;
        cout <<"--Input " << n << " element in reference string--\n";
        for (size_t i = 0; i < n; i++)
            cin >> NewArr[i];
    }
    cout <<"--- Page Replacement algorithm ---\n";
    cout <<"Input page frames:";
    cin >>nOpf;
    cout << "--- Page Replacement algorithm ---\n";
    cout <<"1. FIFO algorithm\n";
    cout <<"2. OPT algorithm\n";
    cout <<"3. LRU algorithm\n";
    cin >> option2;
    init();
    if (option2 == 1)
        FIFOAlgorithmStart();
    else if (option2 == 2)
        OTPAlgorithmStart();
    else
        LRUAlgorithmStart();
    for (size_t j = 0; j < n; j++)
        cout << NewArr[j] << " ";
    cout << endl;
    for (size_t i = 0; i < nOpf; i++){
        for (size_t j = 0; j < n; j++){
            if (PageFrames[i][j] > -1)
                cout << PageFrames[i][j] << " ";
            else
                cout << " ";
        }
        cout << endl;
    }
    for (size_t j = 0; j < n; j++)
        cout << PageFaultRowExtend[j] << " ";
    cout << endl;
    cout << "Number of page Fault: " << PageFaultCount;
    return 0;
} 