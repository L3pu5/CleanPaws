#include <string>
#include <map>
#include <vector>

// 
// bOpts: Bunny command parsing.
//      https://github.com/L3pu5/bOpts
//      By L3pu5, L3pu5_Hare, Lepus Hare.

namespace bOpts{

    int bOpts_FLOATINGCOUNT = 0;
    int bOpts_FLAGCOUNT = 0;
    std::map <std::string, std::string> bOpts_MAP = std::map<std::string, std::string>();
    std::map <std::string, std::vector<std::string>> bOpts_FLAGMAP = std::map <std::string, std::vector<std::string>>();

    struct dOptException : std::exception{
        std::string InternalErrorMessage;
        const char * what() const throw (){
            return InternalErrorMessage.c_str();
        }
        dOptException(std::string _error){
            InternalErrorMessage = _error;
        }
    };

    //SetOpts takes in a string as the 'root' opt, and a vector of optional opts that will collapse into the root.
    void SetOpts(std::string _flag, std::vector<std::string> _variants){
        bOpts_FLAGMAP.insert({_flag, _variants});
        bOpts_FLAGCOUNT++;
    }

    void MakeOpts(int argc, char* _argv[]){
        //Internal variables
        bool _priorSwitch = 0;
        std::string _key = "";
        //Main Loop.
        for (int i = 1; i < argc; i++)
        {
            //Read the string
            std::string _argument(_argv[i]);
            //Check if prior was a switch
            if(_priorSwitch == 0){
                //If not, is this a switch?
                if(_argument.at(0) == '-'){
                    _priorSwitch = 1;
                    _key = _argument.substr(1);
                    //If we have any variant flags, check all of them.
                    if(bOpts_FLAGCOUNT > 0){
                        //Loop over each of the flags, see if we belong to any of them.
                        for(auto const& [flag, variants] : bOpts_FLAGMAP){
                            if(_key == flag)
                                goto bopt_exit;
                            for(std::string _variant : variants){
                                if(_key == _variant)
                                    _key = flag;
                            }
                        }
                        
                    }
                    bopt_exit:
                    continue;
                }
                _key += "Floating_";
                _key += std::to_string(bOpts_FLOATINGCOUNT);
                //If this is a value and not a switch, feed it into the map as one of the floating values.
                bOpts_MAP.insert({_key, _argument});
                bOpts_FLOATINGCOUNT++;
                _key = "";
                continue;
            }
            //Prior was a switch
            //Read it directly
            bOpts_MAP[_key] = _argument;
            _key = "";
            _priorSwitch = 0;
            continue;
        }

        
    }

    //Returns std::string of the argument provided for a given switch. 
    //  If the key/switch does not exist, throws an exception. 
    std::string GetOpt(std::string _key){
        //Check to see if the _key is witihn the map.
        if(bOpts_MAP.count(_key)){
            return bOpts_MAP[_key];
        }
        throw dOptException("GetOpt(std::string _key) did not locate _key " + _key);
        //return "NULL";
    }

    //Returns the string of the command line arg entered, else returns nullptr
    std::string GetOpt(int _index, bool head=true){
        //Counting from the left
        if(head){
            //Check so make sure _index+1 does not exceed argc
            if(_index + 1 >= __argc)
                throw dOptException("GetOpt(int _index, bool head) exceeded argc for _index" + std::to_string(_index));        
            return std::string(__argv[_index+1]);
        }
        //Counting from the right
        //Check so make sure _index+1 does not go lower than 1
        if(__argc - _index - 1<= 0)
                throw dOptException("GetOpt(int _index, bool head): _index = " + std::to_string(_index) + " was lower than 1");
        return std::string(__argv[__argc - _index - 1]);
    }

    //Returns the nth floating option, the option without a flag prior at location 'index'
    //  If there are not enough options, throws and exception.
    //  This is 0 indexed, so the first instance is at GetFloatingOpt(0).
    std::string GetFloatingOpt(int _index){
        if(_index >= bOpts_FLOATINGCOUNT)
            throw dOptException("GetFloatingOpt(" + std::to_string(_index) + "): _index = " + std::to_string(_index) + " not found.");
        return bOpts_MAP["Floating_" + std::to_string(_index)];
    }

}