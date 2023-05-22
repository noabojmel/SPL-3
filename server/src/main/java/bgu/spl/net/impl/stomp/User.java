package bgu.spl.net.impl.stomp;

import java.util.Map;
import java.util.HashMap;

//this class saves the data base of a single user
public class User {
    private String username;
    private String password;
    private int uniqeId;//the id that the server gave the user
    private HashMap<Integer,String> gamesSubspriction;//the topict that this user registerd to
    private boolean connectedSuccessfully =false;
    


//constructor
public User(String userName, String password, int uniqeId){
    this.username=userName;
    this.password=password;
    this.uniqeId=uniqeId;
    this.gamesSubspriction=new HashMap<>();
}

//getters
public String getUsername(){
    return username;
}
public String getPassword(){
    return password;
}
public int getUniqe(){
    return uniqeId;
}
public HashMap<Integer,String> getSubsprictionList(){
    return gamesSubspriction;
}
public boolean connectedSuccessfully(){
    return connectedSuccessfully;
}

//get sub number by game name
public Integer getSubNumByGameName(String game){
    for(Map.Entry<Integer,String> entry : gamesSubspriction.entrySet()){
        if(entry.getValue().equals(game)){
            return entry.getKey();
        }
    }
    return null;
} 


//add a new game to sub list
public void addSub(String game,Integer subscriptionId){    if(!gamesSubspriction.containsValue(game)){
        gamesSubspriction.put(subscriptionId,game);
    }
}

//remove game from list
public void removeSub(Integer subscriptionId){
    gamesSubspriction.remove(subscriptionId);
}

public void setConnectedSuccessfully(boolean before){
    connectedSuccessfully=before;
}


}
