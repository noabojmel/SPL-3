package bgu.spl.net.impl.stomp;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;

import bgu.spl.net.srv.ConnectionImpl;

public class DataBase { 
    private CopyOnWriteArrayList<User> allUsers;//thread safe list
    private static AtomicInteger userCounter; //for tracking how many users we have
    private static AtomicInteger msgCount;//for generating messeges id;
    private ConnectionImpl<String> connectios;//stomp protocol have strings
    private ConcurrentHashMap<String,CopyOnWriteArrayList<Integer>> games_subscribedUsers;//map games to subscribes that are registerd to 'key' game
    private ConcurrentHashMap<Integer,User> activeUsers_id;//maps active users to their connection id
    private static class singelton{private static DataBase dataInstance= new DataBase();}//for connections

    //constructor
    public DataBase(){
        this.allUsers =new CopyOnWriteArrayList<>();
        userCounter=new AtomicInteger(0);
        this.connectios=ConnectionImpl.getInstance();
        games_subscribedUsers=new ConcurrentHashMap<>();
        activeUsers_id=new ConcurrentHashMap<>(); 
        msgCount=new AtomicInteger(0);
    }

    //singelton instance
    public static DataBase getInstance(){
        return singelton.dataInstance;
    }

    public boolean joinGame (Integer connectionId, String game, Integer subIdByUser){
        CopyOnWriteArrayList<Integer> tmpArray = new CopyOnWriteArrayList<>();
        tmpArray.add(connectionId);
        if(games_subscribedUsers.containsKey(game)){// that game exists
            if (!games_subscribedUsers.get(game).contains(connectionId)) {            //checks if already contains id
                games_subscribedUsers.get(game).add(connectionId);                   // ADDING client id to the game
                activeUsers_id.get(connectionId).addSub(game, subIdByUser);   // adding subId by user to his user instance
            } else
                return false; //this user has already subscribed to this topic
        }
        else{//this game is new
            games_subscribedUsers.put(game,tmpArray);
        }
        activeUsers_id.get(connectionId).addSub(game, subIdByUser);   // adding subId by user to his user instance
        return true; // subscribed success
    }

    public boolean exitGame (Integer connectionId, Integer subIdByUser){ 

        String game = activeUsers_id.get(connectionId).getSubsprictionList().get(subIdByUser);
        if(game!=null){        //The user has this game in his subscription list
            games_subscribedUsers.get(game).remove(connectionId);                      // removes the user (id) from his game array
            activeUsers_id.get(connectionId).removeSub(subIdByUser);      // remove game from his user instance
            return true;
        }
        return false;
    }

    public String login(int connectionId, String userName, String password){
        synchronized (allUsers) {         // so 2 users will not create a user with the same username
            for (User u : allUsers) {
                if (u.getUsername().equals(userName)) {  // found the name of the user
                    if(u.getPassword().equals(password)){
                        if(activeUsers_id.containsKey(connectionId)){
                            //already connected
                            return "User already logged in";
                        }
                        else{
                            //connected succes
                            activeUsers_id.put(connectionId, u);                     //adding curr user & id to the map
                            u.setConnectedSuccessfully(true);                           
                            return "connected";
                        }
                    }
                    else{
                        //wrong password
                        return "Wrong password";
                    }
                }
            }
            //open new user
            int tmpCount = userCounter.incrementAndGet();                      //inc the counter of users
            User user = new User(userName, password, tmpCount);          //create new user
            allUsers.add(tmpCount-1, user);                              //adding new user to the all user array to the end
            activeUsers_id.put(connectionId, user);                       //adding new user & connectionId to the ActiveUserMap
            user.setConnectedSuccessfully(true);                            
            return "connected";
        }
}

public boolean logout (Integer connectionID){
    for(CopyOnWriteArrayList<Integer> currGames : games_subscribedUsers.values()){
        currGames.remove(connectionID);                              // removing the conn id from each topic list
    }
    activeUsers_id.get(connectionID).getSubsprictionList().clear();;                 //removes all topics that exits of this user
    activeUsers_id.get(connectionID).setConnectedSuccessfully(false);  
    activeUsers_id.remove(connectionID);                                      //user is not active anymore
    return true;
}

public CopyOnWriteArrayList<Integer> getGamesList (String game){
    return games_subscribedUsers.get(game);

}

public int getSubIdConnectionId(Integer connectionId,String game){
    return this.activeUsers_id.get(connectionId).getSubNumByGameName(game);
}

public int getUniqeMsgId(){
    return msgCount.incrementAndGet(); 
}
}
