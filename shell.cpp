#include <termios.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>

#include <dirent.h>
using namespace std;


#define STDIN_FILENO 0
#define STDOUT_FILENO 1


void shell();
vector<pid_t>child_process;
vector<string>file_names;

void signal_callback_handler2(int signum) {
   //cout << "Caught signal " << signum << endl;
   // Terminate program
   
for(auto v:file_names)
{

char filename[1000];
for(int i=0;i<1000;i++)
filename[i]='\0';
for(int i=0;i<v.size();i++)
filename[i]=v[i];

int status = remove(filename);
  

} 
file_names.clear();   
   
   for(auto v:child_process)
{
kill(v, SIGTERM);
}
child_process.clear();
  shell();
   //exit(EXIT_FAILURE);
}

void signal_callback_handler(int signum) {
   //cout << "Caught signal " << signum << endl;
   // Terminate program
   shell();
   //exit(EXIT_FAILURE);
}

void signal_callback_handler1(int signum) {
   //cout << "Caught signal " << signum << endl;
   // Terminate program
   cout<<"Process moved to background"<<"\n";
   int status;
   pid_t pid, wpid;
   pid = fork();
   if(pid==0){

   }
   else{
        do{
            wpid = waitpid(pid,&status,WUNTRACED);
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
   }
   shell();
   //exit(EXIT_FAILURE);
}

vector<string> tokenizer(string s, char c){
    /*Function to tokenize string s using delimiter c*/
    
    stringstream sss(s);
    string temp;
    vector<string> tkns;
    while (getline(sss, temp, c))
    {
        tkns.push_back(temp);
    }
    return tkns;
}


int lcs(string  X, string Y, int m, int n)
{
    
 
    int LCSuff[m + 1][n + 1];
    int result = 0; 
 
  
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
         
            if (i == 0 || j == 0)
                LCSuff[i][j] = 0;
 
            else if (X[i - 1] == Y[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
                result = max(result, LCSuff[i][j]);
            }
            else
                LCSuff[i][j] = 0;
        }
    }
    return result;
}

deque<string>qc;
vector <string> built_ins {"cd", "exit","history"};



int shell_cd(vector <string> args){
   
    if(chdir(args[1].c_str()) !=0){
        perror("Wrong path");
    }
    return EXIT_SUCCESS;
}



int shell_exit(vector <string> args){
   
    exit(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

int shell_history(vector<string>args)
{

if(qc.size()<=1000)
{
for(int i=0;i<qc.size();i++)
cout<<qc[i]<<"\n";
}
else
{
for(int i=qc.size()-1000;i<qc.size();i++)
cout<<qc[i]<<"\n";

}

int c;   
    
    int i=0;
    static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);         

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*This is your part:
    I choose 'e' to end input. Notice that EOF is also turned off
    in the non-canonical mode*/

   c=getchar();
   if(c==18)
   {
   char buf[100];
    int i;
    for(i=0;i<100;i++)buf[i]='\0';
    
   cout<<"Enter the prompt term"<<"\n";
   i=0;
   while((c=getchar())!= '\n'){      
             
        buf[i] = (char)c;i++;}
   
   
 string s1="";
 for (int i = 0; i < strlen(buf); i++) {
            s1 = s1 + buf[i];
        }
 

int state=0;
vector<string>ans;
int n=qc.size();
vector<int>match(n,0);
int mx=-1;
int lans=-1;

 i=0;
    while(i!=n)
    {string temp=qc[i];
 
        if(temp==s1)
        {
            state=1;
            ans.push_back(temp);
            break;


        }
        else
        {   int m=s1.size();
            int nn=temp.size();

        int k=lcs(s1, temp, m, nn);
        if(k>=2)
        {
            state=2;
            match[i]=k;
            mx=max(mx,k);
            
        }


        }
        i++;
        
    }
    
    
    
        if(state==0)
        {
            cout<<"No match for search term in history"<<"\n";

        }
        else if(state==1)
        cout<<ans[0]<<"\n";
        else
        {
            for(int i=0;i<n;i++)
            {
            
            if(match[i]==mx)
            cout<<qc[i]<<"\n";
            }
        }
        

    }
    if(qc.size()==10000)
        qc.pop_front();
       
        qc.push_back("history");


   
   
   
   
   
    
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    

return EXIT_SUCCESS;
}


int (*built_in_func[]) (vector <string>) = {&shell_cd, &shell_exit,&shell_history};


class My_Shell_Command{
    
    public:
        string cmd;             
        vector<string> args;    
        string inpf;             
        string outf;            
        bool bkgd;              
        int built_in;
        My_Shell_Command(string);
        void separate();
        void input_output();
        void input_output1(int);
        void exec();
};

My_Shell_Command::My_Shell_Command(string cmd){
    /*Class Constructor*/
    this->cmd = cmd;
    
    bkgd = false;
    built_in = -1;
    inpf = "";
    outf = "";
}

void My_Shell_Command::separate(){
   
    vector<string> tokens = tokenizer(cmd,' ');

    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];
        if (token != "")
        {
            if (token == "&")
            {
                bkgd = true;
            }
            
            else if (token == ">")
            {
                if (i== tokens.size()-1)
                {
                    cout << "No Output file given!\n";
                    exit(EXIT_FAILURE);
                }
                outf = tokens[++i];
            }
            else if (token == "<")
            {
                if (i  == tokens.size()-1)
                {
                    cout << "No Input file given !\n";
                    exit(EXIT_FAILURE);
                }
                inpf = tokens[++i];
            }
            else{
                args.push_back(token);
            }
        }
    }
    
    for(int i=0; i<built_ins.size(); i++){
        if(strcmp(args[0].c_str(), built_ins[i].c_str()) == 0)
            built_in = i;
    }
    return;
}

void My_Shell_Command::input_output(){
    
    if (inpf != ""){
        int fd = open(inpf.c_str(), O_RDONLY); 
        if (fd < 0){
            cout << "Failure to open file!" << endl;
            exit(EXIT_FAILURE);
        }
        int status = dup2(fd, STDIN_FILENO); 
        if (status < 0){
            cout << "Input redirection failed !" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (outf != ""){
        int fd = open(outf.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); 
        if (fd < 0){
            cout << "Failure to open file!"<< endl;
            exit(0);
        }
        int status = dup2(fd, STDOUT_FILENO); 
        if (status < 0){
            cout << "Output redirection failed" << endl;
            exit(0);
        }
    }
}

void My_Shell_Command::input_output1(int fdtemp){
    
    if (inpf != ""){
        int fd = open(inpf.c_str(), O_RDONLY); 
        if (fd < 0){
            cout << "Failure to open file!" << endl;
            exit(EXIT_FAILURE);
        }
        int status = dup2(fd, STDIN_FILENO); 
        if (status < 0){
            cout << "Input redirection failed !" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (outf != ""){
        int fd = open(outf.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); 
        if (fd < 0){
            cout << "Failure to open file!" << endl;
            exit(0);
        }
        int status = dup2(fd, STDOUT_FILENO); 
        if (status < 0){
            cout << "Output redirection failed" << endl;
            exit(0);
        }
    }
   if(outf=="")
{

       int status = dup2(fdtemp, STDOUT_FILENO); 
        if (status < 0){
            cout << "Output redirection failed" << endl;
            exit(0);
        }


}



}

void My_Shell_Command::exec(){
    
    char *arg2[args.size() + 1];
    int i = 0;
    for (string a : args){
      
        arg2[i++] = (char *)(a.c_str());
    }
    arg2[i++] = NULL;
    arg2[0] = (char *)(args[0].c_str());
    if(execvp(arg2[0], arg2)==-1){
        cout<<"Can not execute Command\n";
    }
}


vector<My_Shell_Command> split(string line){
    
    vector<string> tkns = tokenizer(line,'|');
    vector<My_Shell_Command> commands;
    for (auto token : tkns){
        My_Shell_Command cmd = My_Shell_Command(token);
        cmd.separate();
        commands.push_back(cmd);
    }
    return commands;
}


void run(vector<My_Shell_Command>commands,int status,int fd_temp)
{
 
        
        
            
            
            if (commands.size() == 1){ 
               
                if(commands[0].built_in != -1)
                {
                    (*built_in_func[commands[0].built_in])(commands[0].args);
                }
                else{
                    pid_t pid = fork(); 
                    pid_t wpid;
                    if (pid == 0){
                        commands[0].input_output1(fd_temp);
                        commands[0].exec();
                    
                        exit(EXIT_FAILURE); 
                    }
                    else{
                        if (commands[0].bkgd == false){
                            //cout<<">>";
                            // wait(&status); //cout<<">>";// wait if child not a background process
                            do{
                                wpid = waitpid(pid,&status,WUNTRACED);
                            }while(!WIFEXITED(status) && !WIFSIGNALED(status));
                        }
                        else{
                            cout << "[BG] " << pid << endl;
                        }
                        //close(pid);
                    }  
                    //close(pid); 
                }
            }
            else {
                int num_cmds = commands.size();
                int newFD[2], FD[2];
                for (int i = 0; i < num_cmds; i++){ 
                    if (i + 1 < num_cmds)
                        pipe(newFD); 
                    pid_t pid = fork(); 
                    if (pid == 0){
                        if (i == 0 || i + 1 == num_cmds){ 
                            commands[i].input_output1(fd_temp);
                        }
                        if (i){
                            dup2(FD[0], 0);
                            close(FD[0]);
                            close(FD[1]);
                        }
                        if (i + 1 < num_cmds){ 
                            close(newFD[0]);
                            dup2(newFD[1], 1);
                            close(newFD[1]);
                        }
                        commands[i].exec();
                        exit(EXIT_FAILURE);
                    }
                    else{
                        if (commands[i].bkgd != false){
                            wait(NULL); 
                        }
                    }

                    if (i != 0){ 
                        close(FD[0]);
                        close(FD[1]);
                    }
                    if (i + 1 != num_cmds){ 
                        FD[0] = newFD[0], FD[1] = newFD[1];
                    }
                }

                if (commands.back().bkgd == false){
                    while (wait(&status) > 0);
                }
            }
        }
        


int shell_multiwatch(string args)
{
signal(SIGINT, signal_callback_handler);
vector<string>com=tokenizer(args,',');
for(int i=0;i<com.size();i++)
{
string temp=com[i].substr(1,com[i].size()-2);
com[i]=temp;
}
vector<vector<My_Shell_Command>>cmd;
for(int i=0;i<com.size();i++)
{
vector<My_Shell_Command> cmds = split(com[i]);
cmd.push_back(cmds);


}
time_t result = time(nullptr);
int status;
int fdmax=-2;
string name=".temp.PID";
string extension=".txt";


fd_set read_fds;
map<int,int>mp;
vector<int>fd_write(cmd.size());
vector<int>fd_read(cmd.size());



    for (int i=0; i<cmd.size(); i++) {
     
        pid_t slave = fork();
       
        if (slave==0) /* only execute this if child */
        {
           // std::cout << "Slave " << i << ": ";
            child_process.push_back(getpid());
    
            string file_name="";
file_name+=name;
string str= to_string(i);  
file_name+=str;
file_name+=extension;

file_names.push_back(file_name);


char filename[1000];
for(int i=0;i<1000;i++)
filename[i]='\0';
for(int i=0;i<file_name.size();i++)
filename[i]=file_name[i];



 

  // Close the file
            
            int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, S_IRWXU); // open file in write/read only
            int fd2 = open(filename, O_RDONLY);
            
            fd_read[i]=fd2;
            mp[fd2]=i;

            
            
            run(cmd[i],status,fd);
            
            
            //std::cout << "Slave " << i << ": Done" << std::endl;
            
        }
    }
    
       
while(1)
{ 
        signal(SIGINT, signal_callback_handler2);

        FD_ZERO(&read_fds);
        
        int fdmax=fd_read[0];
        for(int i=0;i<fd_read.size();i++)
        {
        FD_SET(fd_read[i],&read_fds);
        fdmax=max(fdmax,fd_read[i]);
        }    
    


if (select(fdmax+1, &read_fds, 0, 0, 0) == -1) {
perror("select");
exit(1);
}
else
{

for(int i = 0; i <cmd.size(); i++) {

if (FD_ISSET(fd_read[i], &read_fds)) 
{ // we got one!!
int nbytes;
char buff[1000];

if ((nbytes = read(fd_read[i], buff, 1000)) > 0) {


           time_t t;   // not a primitive datatype
           time(&t);
            cout<<"\n"<<"\n"<<com[i]<<", "<<"  ";
            cout<<"current time  :";
            //cout << asctime(localtime(&result))
            //  << result << "\n";
            //printf("\nThis program has been writeen at (date and time): %s", ctime(&t));
            cout<<ctime(&t)<<"\n";
            cout<<"<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-"<<"\n";  
            cout<<"Output"<<i+1<<"\n";
            cout<<"->->->->->->->->->->->->->->->->->->->"<<"\n";
            buff[nbytes]='\0';
            
write(1,buff,strlen(buff));

}
}
}
}





}
    
    
    
    for (int i=0; i<cmd.size(); i++) {
        wait(&status);
    }
    


    return EXIT_SUCCESS;
}



string tab_op(char* buf, string line){
    string s = "";
            int j,m,k;
            if(buf[strlen(buf)-1] != ' '){
                for(j=strlen(buf)-1;j>=0;j--){
                    if(buf[j]==' ')break;
                }
                j+=1;
                for(m=j;m<strlen(buf);m++){
                    s+=buf[m];
                }
                //cout<<endl<<s;
                vector<string> p,q;
                struct dirent *d;
                DIR *dr;
                dr = opendir(".");
                if(dr!=NULL)
                {
                    //cout<<"List of Files & Folders:-\n";
                    for(d=readdir(dr); d!=NULL; d=readdir(dr))
                    {
                        //cout<<d->d_name<<endl;
                        p.push_back(d->d_name);
                    }
                    closedir(dr);
                }
                else
                    cout<<"\nError Occurred!";
                m=0;
                for(j=0;j<s.size();j++){
                    for(k=0;k<p.size();k++){
                        string z = p[k];
                        if(z[j]==s[j]){
                            q.push_back(z);
                        }
                    }
                    p = q;
                    q.clear();
                }
                if(p.size()==1){
                    for(j=s.size();j<p[0].size();j++){
                        string z = p[0];
                        cout<<z[j];
                        line+=z[j];
                    }
                    cout<<" ";
                    line+=" ";
                }
                else if(p.size()>1){
                    cout<<"\n";
                    int w=1;
                    for(w=1;w<=p.size();w++){
                        cout<<w;
                        cout<<". "<<p[w-1]<<" ";
                    }
                    cout<<"\nEnter your choice: ";
                    int c1;   
                    char buf1[100];
                    int i;
                    //cin>>buf1[0];
                    for(i=0;i<100;i++)buf1[i]='\0';
                    i=0;
                    

                    

                    while((c1=getchar())!= '\n'){      
                        //putchar(c);
                        
                        if(c1 == 127){
                            i--;buf1[i]='\0';
                            const char delbuf[] = "\b \b";
                            write(STDOUT_FILENO, delbuf, strlen(delbuf));
                        }
                        else{
                            putchar(c1);  
                            buf1[i] = (char)c1;i++;    
                        }            
                    }
                    if((char)c1 == '\n')putchar(c1);
                    /*restore the old settings*/
                    
                    //cin>>w;
                    w=0;int l,k=1;
                    for(l=strlen(buf1)-1;l>=0;l--){
                        w = (buf1[l]-'0')*k + w;
                        k*=10;
                    }
                    for(j=s.size();j<p[w-1].size();j++){
                        string z = p[w-1];
                        //cout<<z[j];
                        line+=z[j];
                    }
                    //cout<<" ";
                    line+=" ";
                    cout<<"\n>>"<<line;
                }
                return line;
            }
            else return line;
}



void shell(){
    /*main loop*/
    int status;
    string line;
    do{
        signal(SIGINT, signal_callback_handler);
        signal(SIGTSTP, signal_callback_handler1);
        printf(">> ");  //shell prompt
        //getline(cin, line);
        int tab=0;
        int c;   
        char buf[100];
        int i;
        for(i=0;i<100;i++)buf[i]='\0';
        i=0;
        static struct termios oldt, newt;

        /*tcgetattr gets the parameters of the current terminal
        STDIN_FILENO will tell tcgetattr that it should write the settings
        of stdin to oldt*/
        tcgetattr( STDIN_FILENO, &oldt);
        /*now the settings will be copied*/
        newt = oldt;

        /*ICANON normally takes care that one line at a time will be processed
        that means it will return if it sees a "\n" or an EOF or an EOL*/
        newt.c_lflag &= ~(ICANON | ECHO);         

        /*Those new settings will be set to STDIN
        TCSANOW tells tcsetattr to change attributes immediately. */
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);

        /*This is your part:
        I choose 'e' to end input. Notice that EOF is also turned off
        in the non-canonical mode*/

        while((c=getchar())!= '\n'){      
            //putchar(c);
            if((char)c == '\t'){
                tab=1;break;
            }    
            if(c == 127){
                i--;buf[i]='\0';
                const char delbuf[] = "\b \b";
                write(STDOUT_FILENO, delbuf, strlen(delbuf));
            }
            else{
                putchar(c);  
                buf[i] = (char)c;i++;    
            }       
        }
        //cout<<"\n"<<tab<<"\n";
        if((char)c == '\n')putchar(c);
        /*restore the old settings*/
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
        line = "";
        for (int i = 0; i < strlen(buf); i++) {
            line = line + buf[i];
        }
        if(!line.empty()&&(line!="history"))
        {
        if(qc.size()==10000)
        qc.pop_front();
       
        qc.push_back(line);
        
        }
        
        if(tab==0){
        string r=line.substr(0,10);
        string var="multiWatch";
            
            
            if((line.length()>14)&&(var==r))
            {
            
            
            int z=line.length();
            string temp=line.substr(12,z-13);
        
            
            shell_multiwatch(temp);



            }
            else{
        
            vector<My_Shell_Command> commands = split(line);
            if(commands.size() == 0){
                continue;
            }
            if (commands.size() == 1)
            { 
                if(commands[0].built_in != -1){
                    (*built_in_func[commands[0].built_in])(commands[0].args);
                }
                else{
                    pid_t pid = fork(); 
                    pid_t wpid;
                    if (pid == 0){
                        commands[0].input_output(); 
                        commands[0].exec();
                        /*if (cmds[0].bkg == 0){
                            cout<<">>";
                        }*/
                        //close(pid);
                        exit(EXIT_FAILURE); 
                    }
                    else{
                        if (commands[0].bkgd == false)
                        {
                            //cout<<">>";
                            // wait(&status); //cout<<">>";// wait if child not a background process
                            do{
                                wpid = waitpid(pid,&status,WUNTRACED);
                            }while(!WIFEXITED(status) && !WIFSIGNALED(status));
                        }
                        else
                        {
                            cout << "[BG] " << pid << endl;
                        }
                        //close(pid);
                    }  
                    //close(pid); 
                }
            }
            else {
                int num_cmds = commands.size();
                int newFD[2], FD[2];
                for (int i = 0; i < num_cmds; i++){ 
                    if (i + 1 < num_cmds)
                        pipe(newFD); 
                    pid_t pid = fork(); 
                    if (pid == 0){
                        if (i == 0 || i + 1 == num_cmds)
                        { 
                            commands[i].input_output();
                        }
                        if (i)
                        {
                            dup2(FD[0], 0);
                            close(FD[0]);
                            close(FD[1]);
                        }
                        if (i + 1 < num_cmds)
                        { 
                            close(newFD[0]);
                            dup2(newFD[1], 1);
                            close(newFD[1]);
                        }
                        commands[i].exec();
                        exit(EXIT_FAILURE);
                    }
                    else{
                        if (commands[i].bkgd != false){
                            wait(NULL); 
                        }
                    }

                    if (i != 0)
                    { 
                        close(FD[0]);
                        close(FD[1]);
                    }
                    if (i + 1 != num_cmds)
                    { 
                        FD[0] = newFD[0], FD[1] = newFD[1];
                    }
                }

                if (commands.back().bkgd == false){
                    while (wait(&status) > 0);
                }
            }
        }
        }
        else{
            string s = "";
            int j,m,k;
            if(buf[strlen(buf)-1] != ' '){
                for(j=strlen(buf)-1;j>=0;j--){
                    if(buf[j]==' ')break;
                }
                j+=1;
                for(m=j;m<strlen(buf);m++){
                    s+=buf[m];
                }
                //cout<<endl<<s;
                vector<string> p,q;
                struct dirent *d;
                DIR *dr;
                dr = opendir(".");
                if(dr!=NULL)
                {
                    //cout<<"List of Files & Folders:-\n";
                    for(d=readdir(dr); d!=NULL; d=readdir(dr))
                    {
                        //cout<<d->d_name<<endl;
                        p.push_back(d->d_name);
                    }
                    closedir(dr);
                }
                else
                    cout<<"\nError Occurred!";
                m=0;
                for(j=0;j<s.size();j++){
                    for(k=0;k<p.size();k++){
                        string z = p[k];
                        if(z[j]==s[j]){
                            q.push_back(z);
                        }
                    }
                    p = q;
                    q.clear();
                }
                if(p.size()==1){
                    for(j=s.size();j<p[0].size();j++){
                        string z = p[0];
                        cout<<z[j];
                        line+=z[j];
                    }
                    cout<<" ";
                    line+=" ";
                }
                else if(p.size()>1){
                    cout<<"\n";
                    int w=1;
                    for(w=1;w<=p.size();w++){
                        cout<<w;
                        cout<<". "<<p[w-1]<<" ";
                    }
                    cout<<"\nEnter your choice: ";
                    int c1;   
                    char buf1[100];
                    int i;
                    //cin>>buf1[0];
                    for(i=0;i<100;i++)buf1[i]='\0';
                    i=0;
                    static struct termios oldt, newt;

                    /*tcgetattr gets the parameters of the current terminal
                    STDIN_FILENO will tell tcgetattr that it should write the settings
                    of stdin to oldt*/
                    tcgetattr( STDIN_FILENO, &oldt);
                    /*now the settings will be copied*/
                    newt = oldt;

                    /*ICANON normally takes care that one line at a time will be processed
                    that means it will return if it sees a "\n" or an EOF or an EOL*/
                    newt.c_lflag &= ~(ICANON | ECHO);         

                    /*Those new settings will be set to STDIN
                    TCSANOW tells tcsetattr to change attributes immediately. */
                    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

                    /*This is your part:
                    I choose 'e' to end input. Notice that EOF is also turned off
                    in the non-canonical mode*/

                    while((c1=getchar())!= '\n'){      
                        //putchar(c);
                        
                        if(c1 == 127){
                            i--;buf1[i]='\0';
                            const char delbuf[] = "\b \b";
                            write(STDOUT_FILENO, delbuf, strlen(delbuf));
                        }
                        else{
                            putchar(c1);  
                            buf1[i] = (char)c1;i++;    
                        }            
                    }
                    if((char)c1 == '\n')putchar(c1);
                    /*restore the old settings*/
                    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
                    //cin>>w;
                    w=0;int l,k=1;
                    for(l=strlen(buf1)-1;l>=0;l--){
                        w = (buf1[l]-'0')*k + w;
                        k*=10;
                    }
                    for(j=s.size();j<p[w-1].size();j++){
                        string z = p[w-1];
                        //cout<<z[j];
                        line+=z[j];
                    }
                    //cout<<" ";
                    line+=" ";
                    cout<<"\n>>"<<line;
                }
            }
            //cout<<endl<<line;
            int c1;   
            char buf1[100];
            int i;
            //cin>>buf1[0];
            for(i=0;i<100;i++)buf1[i]='\0';
            i=0;
            static struct termios oldt, newt;

            /*tcgetattr gets the parameters of the current terminal
            STDIN_FILENO will tell tcgetattr that it should write the settings
            of stdin to oldt*/
            tcgetattr( STDIN_FILENO, &oldt);
            /*now the settings will be copied*/
            newt = oldt;

            /*ICANON normally takes care that one line at a time will be processed
            that means it will return if it sees a "\n" or an EOF or an EOL*/
            newt.c_lflag &= ~(ICANON | ECHO);         

            /*Those new settings will be set to STDIN
            TCSANOW tells tcsetattr to change attributes immediately. */
            tcsetattr( STDIN_FILENO, TCSANOW, &newt);

            /*This is your part:
            I choose 'e' to end input. Notice that EOF is also turned off
            in the non-canonical mode*/

            while((c1=getchar())!= '\n'){      
                //putchar(c);
                   
                if(c1 == 127){
                    i--;buf1[i]='\0';
                    const char delbuf[] = "\b \b";
                    write(STDOUT_FILENO, delbuf, strlen(delbuf));
                }
                else if(c1=='\t'){
                    //cout<<line;
                    line = tab_op(buf1,line);
                    for(int ik=0;ik<100;ik++)buf1[ik]='\0'; i =0;
                }
                else{
                    putchar(c1);  
                    buf1[i] = (char)c1; 
                    line = line + buf1[i];i++;   
                }               
            }
            if((char)c1 == '\n')putchar(c1);
            /*restore the old settings*/
            tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
            /*for (int i = 0; i < strlen(buf1); i++) {
                line = line + buf1[i];
            }*/
            //cout<<buf1;
             string r=line.substr(0,10);
        string var="multiWatch";
            
            
            if((line.length()>14)&&(var==r))
            {
            
            
            int z=line.length();
            string temp=line.substr(12,z-13);
        
            
            shell_multiwatch(temp);



            }
            else
            {
            vector<My_Shell_Command> commands = split(line);
            if(commands.size() == 0){
                continue;
            }
            if (commands.size() == 1){ // single My_Shell_Command
                /*check for builtins*/
                if(commands[0].built_in != -1){
                    (*built_in_func[commands[0].built_in])(commands[0].args);
                }
                else{
                    pid_t pid = fork(); // child process
                    pid_t wpid;
                    if (pid == 0){
                        commands[0].input_output(); // execute My_Shell_Command
                        commands[0].exec();
                        /*if (cmds[0].bkg == 0){
                            cout<<">>";
                        }*/
                        //close(pid);
                        exit(EXIT_FAILURE); // exit child process incase of failure
                    }
                    else{
                        if (commands[0].bkgd == false){
                            //cout<<">>";
                            // wait(&status); //cout<<">>";// wait if child not a background process
                            do{
                                wpid = waitpid(pid,&status,WUNTRACED);
                            }while(!WIFEXITED(status) && !WIFSIGNALED(status));
                        }
                        else{
                            cout << "[BG] " << pid << endl;
                        }
                        //close(pid);
                    }  
                    //close(pid); 
                }
            }
            else {
                int num_cmds = commands.size();
                int newFD[2], FD[2];
                for (int i = 0; i < num_cmds; i++){ 
                    if (i + 1 < num_cmds)
                        pipe(newFD); 
                    pid_t pid = fork(); 
                    if (pid == 0){
                        if (i == 0 || i + 1 == num_cmds)
                        { 
                            commands[i].input_output();
                        }
                        if (i)
                        {
                            dup2(FD[0], 0);
                            close(FD[0]);
                            close(FD[1]);
                        }
                        if (i + 1 < num_cmds)
                        { 
                            close(newFD[0]);
                            dup2(newFD[1], 1);
                            close(newFD[1]);
                        }
                        commands[i].exec();
                        exit(EXIT_FAILURE);
                    }
                    else{
                        if (commands[i].bkgd != false)
                        {
                            wait(NULL); 
                        }
                    }

                    if (i != 0)
                    { 
                        close(FD[0]);
                        close(FD[1]);
                    }
                    if (i + 1 != num_cmds){ 
                        FD[0] = newFD[0], FD[1] = newFD[1];
                    }
                }

                if (commands.back().bkgd == false){
                    while (wait(&status) > 0);
                }
            }
        }}
    } while (true);
}

int main(int argc, char **argv)
{
    shell();
    return EXIT_SUCCESS;
}
