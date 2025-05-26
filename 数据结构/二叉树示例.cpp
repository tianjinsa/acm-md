    #include <bits/stdc++.h>
    using namespace std;
    struct TreeNode{
        int val;
        TreeNode *left,*right;
        TreeNode(int a=0):val(a),left(nullptr),right(nullptr){}
    };



    
    TreeNode* maketree(vector<int>&postlist,vector<int>&inlist,int post_start,int post_end,int in_start,int in_end,map<int,int>&mp)
    {
        if (post_start>post_end || in_start>in_end) return nullptr;
        TreeNode* root=new TreeNode(postlist[post_end]);
        int rootindex=mp[root->val];
        int left_size=rootindex-in_start;
        root->left=maketree(postlist,inlist,post_start,post_start+left_size-1,in_start,rootindex-1,mp);
        root->right=maketree(postlist,inlist,post_start+left_size,post_end-1,rootindex+1,in_end,mp);
        return root;
    }
    
    TreeNode* maketreefirst(vector<int>&postlist,vector<int>&inlist)
    {
        map<int,int>mp;
        for (int i=0;i<inlist.size();i++){
            mp[inlist[i]]=i;
        }
        return maketree(postlist,inlist,0,postlist.size()-1,0,inlist.size()-1,mp);
    }
    
    TreeNode* maketree(vector<int>& prelist, vector<int>& inlist, int pre_start, int pre_end, int in_start, int in_end, map<int,int>& mp)
    {
        if(pre_start > pre_end || in_start > in_end) return nullptr;
        TreeNode* root = new TreeNode(prelist[pre_start]);
        int rootindex = mp[root->val];
        int left_size = rootindex - in_start;
        root->left = maketree(prelist, inlist, pre_start+1, pre_start+left_size, in_start, rootindex-1, mp);
        root->right = maketree(prelist, inlist, pre_start+left_size+1, pre_end, rootindex+1, in_end, mp);
        return root;
    }

    TreeNode* maketreefirst(vector<int>& prelist, vector<int>& inlist)
    {
        map<int,int> mp;
        for (int i = 0; i < inlist.size(); i++){
            mp[inlist[i]] = i;
        }
        return maketree(prelist, inlist, 0, prelist.size()-1, 0, inlist.size()-1, mp);
    }
    
    int main()
    {
        int n;
        cin>>n;
        vector <int>postlist(n),inlist(n);
        for (int i=0;i<n;i++) cin>>inlist[i];
        for (int i=0;i<n;i++) cin>>postlist[i];
        TreeNode* root=maketreefirst(postlist,inlist);
        queue<pair<TreeNode*,int>>q;
        q.push({root,0});
        map<int,deque<int>>mp;
        while(!q.empty()){
            auto [node,depth]=q.front();
            q.pop();
            mp[depth].push_back(node->val);
            if (node->left)q.push({node->left, depth + 1});
            if (node->right)q.push({node->right, depth + 1});
        }
        bool fl=0;
        cout<<"R: ";
        for(auto&[depth,nodes]:mp){
            if(fl)cout<<" ";
            else fl=1;
            cout<<nodes.front();
        }
        cout<<endl;
        fl=0;
        cout<<"L: ";
        for(auto&[depth,nodes]:mp){
            if(fl)cout<<" ";
            else fl=1;
            cout<<nodes.back();
        }
        return 0;
    }