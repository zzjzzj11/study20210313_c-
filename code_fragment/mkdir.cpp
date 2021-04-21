#include <iostream>
#include <string>
#include <glob.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

std::string get_containing_dir(std::string const &path)
{
    if(path == "/")
    {
        return std::string{};
    }
    auto last_slash = path.find_last_not_of('/');
    std::cout << "find_last_not_of('/')" << last_slash << std::endl;
    last_slash = path.find_last_of('/',last_slash);
    std::cout << "find_last_of('/')" << last_slash << std::endl;
    if(std::string::npos == last_slash)
    {
        return std::string{};

    }
    last_slash = path.find_last_not_of('/',last_slash);
    std::cout << "find_last_not_of('/')" << last_slash << std::endl;
    return path.substr(0, last_slash + 2);

}

bool file_exists(std::string const& file)
{
    struct stat s = {};
    auto const result = stat(file.c_str(),&s);
    return 0 == result;
}

void make_dir(std::string const& path , mode_t const mode )
{
    auto const status = mkdir(path.c_str(),mode);
    if(-1 == status)
    {
        throw std::system_error{errno , std::system_category(),"mkdir('"+ path + "')"};

    }
}

void make_dirs(std::string const& path , mode_t const mode)
{
    std::string::size_type pos = path.find('/',0);
    while(std::string::npos != pos)
    {
        auto const current_level = path.substr(0,pos);
        if(!file_exists(current_level) && 0 != pos)
        {
            make_dir(current_level,mode);
        }
        ++pos;
        pos = path.find('/',pos);
        
    }
    if(!file_exists(path))
    {
        make_dir(path,mode);
    }

}


int main(int argc , char** argv)
{
    std::string str{"/home/zzj/zzj/git/study20210313_c-/code_fragment/20210411.cpp"};
    std::string tmp = get_containing_dir(str);
    /*
    for(int i = 0; i < str.size() ; i++)
    {
        std::cout << i <<":"<< str[i] << std::endl;
    }
    */
    std::cout << tmp <<std::endl;


    //test makedir
    std::string path{"/home/zzj/zzj/git/study20210313_c-/code_fragment/tmp"};
    make_dirs(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    //得到指定目录下的匹配文件
    glob_t buf;
    std::vector<std::string> vec;
    int i = 0;
    glob("/home/zzj/zzj/git/study20210313_c-/code_fragment/*",GLOB_NOSORT , NULL ,&buf);
    for(i = 0 ; i < buf.gl_pathc; i++)
    {
        std::cout << "num : " << i << " buf.gl_pathv:" << buf.gl_pathv[i] << std::endl;
        std::string tmp = (buf.gl_pathv[i]);
        vec.emplace_back( tmp );
    }

    std::vector<std::string>::iterator ite ;
    for( ite = vec.begin(); ite != vec.end(); ite++)
    {
        std::cout << *ite << std::endl;

    }
    globfree(&buf);
    
    return 0;

}