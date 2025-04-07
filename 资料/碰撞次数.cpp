#include<bits/stdc++.h>
using namespace std;
long double pi=atan(1)*4;
long long ss(long double x){
    if (x <= 0) return -1;
    return ceil(pi/atan(sqrt(x)))-1;
}
int main()
{
    long long a,b;
    while(cin>>a>>b){
        cout<<ss((long double)a/b)<<endl;
    }
    return 0;
}