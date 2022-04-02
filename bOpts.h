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
    int bOpts_TOGGLECOUNT = 0;
    std::map <std::string, std::string> bOpts_MAP = std::map<std::string, std::string>();
    std::map <std::string, std::vector<std::string>> bOpts_FLAGMAP = std::map <std::string, std::vector<std::string>>();
    std::map <std::string, bool> bOpts_TOGGLES;

    struct bOptException : std::exception{
        std::string InternalErrorMessage;
        const char * what() const throw (){
            return InternalErrorMessage.c_str();
        }
        bOptException(std::string _error){
            InternalErrorMessage = _error;
        }
    };

    //SetOpts takes in a std::string as the 'root' opt, and a vector of optional opts that will collapse into the root.
    void SetOpts(std::string _flag, std::vector<std::string> _variants){
        bOpts_FLAGMAP.insert({_flag, _variants});
        bOpts_FLAGCOUNT++;
    }

    //SetToggleOpts takes in a std::vector<std::string> of switches which do NOT expect data after the opt to be used as a toggle.
    void SetToggleOpts(std::vector<std::string> _toggles){
        for(std::string _toggle : _toggles){
            bOpts_TOGGLES.insert({_toggle, 0});
            bOpts_TOGGLECOUNT++;
        }
    }

    void MakeOpts(int argc, char* _argv[]){
        //Internal variables
        bool _priorSwitch = 0;
        std::string _key = "";
        //Main Loop.
        arguments : for (int i = 1; i < argc; i++)
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
                    if(bOpts_FLAGCOUNT){
                        //Loop over each of the flags, see if we belong to any of them.
                        for(auto const& [flag, variants] : bOpts_FLAGMAP){
                            if(_key == flag)
                                goto bopt_exit;
                            for(std::string _variant : variants){
                                if(_key == _variant)
                                {
                                    _key = flag;
                                    goto bopt_exit;
                                }

                            }
                        }
                    }
                    bopt_exit:
                    //If we have Toggle switches, we should flip them.
                    if(bOpts_TOGGLECOUNT){
                        //If this is a toggle
                        if(bOpts_TOGGLES.count(_key)){
                            bOpts_TOGGLES.at(_key) = 1;
                            _priorSwitch = 0;
                            goto escape;
                        }
                    }
                    continue;
                }
                _key += "Floating_";
                _key += std::to_string(bOpts_FLOATINGCOUNT);
                //If this is a value and not a switch, feed it into the map as one of the floating values.
                bOpts_MAP.insert({_key, _argument});
                bOpts_FLOATINGCOUNT++;
                escape:
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
        throw bOptException("GetOpt(std::string _key) did not locate _key " + _key);
        //return "NULL";
    }

    //Retuns bool of the switch provided.
    bool GetToggleOpt(std::string _key){
        if(bOpts_TOGGLES.count(_key)){
            return bOpts_TOGGLES[_key];
        }
        throw bOptException("GetToggleOpt(std::string _key) did not locate _key " + _key); 
    }

    //Returns the string of the command line arg entered, else returns nullptr
    std::string GetOpt(int _index, bool head=true){
        //Counting from the left
        if(head){
            //Check so make sure _index+1 does not exceed argc
            if(_index + 1 >= __argc)
                throw bOptException("GetOpt(int _index, bool head) exceeded argc for _index" + std::to_string(_index));        
            return std::string(__argv[_index+1]);
        }
        //Counting from the right
        //Check so make sure _index+1 does not go lower than 1
        if(__argc - _index - 1<= 0)
                throw bOptException("GetOpt(int _index, bool head): _index = " + std::to_string(_index) + " was lower than 1");
        return std::string(__argv[__argc - _index - 1]);
    }

    //Returns the nth floating option, the option without a flag prior at location 'index'
    //  If there are not enough options, throws and exception.
    //  This is 0 indexed, so the first instance is at GetFloatingOpt(0).
    std::string GetFloatingOpt(int _index){
        if(_index >= bOpts_FLOATINGCOUNT)
            throw bOptException("GetFloatingOpt(" + std::to_string(_index) + "): _index = " + std::to_string(_index) + " not found.");
        return bOpts_MAP["Floating_" + std::to_string(_index)];
    }
        

}
