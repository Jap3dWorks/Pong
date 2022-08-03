//
// Created by Jordi on 6/14/2020.
//
#include <iostream>

#ifndef PONG_LOGGER_H
#define PONG_LOGGER_H

// debug
#ifdef __LOG_DEBUG__
#define LOG_DEBUG(what_) \
    std::cout << "DEBUG: " << what_ << std::endl
#else
#define LOG_DEBUG(message)
#endif

// info
#ifdef __LOG_INFO__
#define LOG_INFO(message) \
    std::cout << "INFO: " << message << std::endl
#else
#define LOG_INFO(what_)
#endif

// warning
#ifdef __LOG_WARNING__
#define LOG_WARNING(message) \
    std::cout << "WARNING: " << message << std::endl
#else
#define LOG_WARNING(what_)
#endif

// Error
#ifdef __LOG_ERROR__
#define LOG_ERROR(message) \
    std::cout << "ERROR: " << message << std::endl
#else
#define LOG_ERROR(what_)
#endif

#endif //PONG_LOGGER_H










//// { Driver Code Starts
////#include <bits/stdc++.h>
//using namespace std;
//#include <stack>
//
//
//
//// } Driver Code Ends
///*
//The structure of the class is as follows
//class _stack{
//stack<int> s;
//int minEle;
//public :
//    int getMin();
//    int pop();
//    void push(int);
//};
//*/
//
//class Solution{
//    int minEle;
//    stack<int> s;
//    stack<int> minims;
//
//public:
//
//    /*returns min element from stack*/
//    int getMin()
//    {
//        //Write your code here
//        return minEle;
//    }
//
//    /*returns poped element from stack*/
//    int pop(){
//        int val = s.top();
//
//        if (minims.size() && val == minEle)
//        {
//            minims.pop();
//            minEle = minims.top();
//        }
//        //Write your code here
//
//        s.pop();
//
//        return val;
//    }
//
//    /*push element x into the stack*/
//    void push(int x){
//        if ((! s.size()) || x > minEle)
//
//        {
//            minEle = x;
//            minims.push(x);
//        }
//        s.push(x);
//
//    }
//};
//
//// { Driver Code Starts.
//
//int main()
//{
//    long long t;
//
//    cin>>t;
//    while(t--)
//    {
//        int q;
//        cin>>q;
//        Solution ob;
//        while(q--){
//            int qt;
//            cin>>qt;
//            if(qt==1)
//            {
//                //push
//                int att;
//                cin>>att;
//                ob.push(att);
//            }
//            else if(qt==2)
//            {
//                //pop
//                cout<<ob.pop()<<" ";
//            }
//            else if(qt==3)
//            {
//                //getMin
//                cout<<ob.getMin()<<" ";
//            }
//        }
//        cout<<endl;
//    }
//    return 0;
//}
//// } Driver Code Ends
