package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import bgu.spl.net.impl.stomp.DataBase;

public class ConnectionImpl<T> implements Connections<T>{

    public Map<Integer,ConnectionHandler<T>> activeClients;//all the active clients
    private AtomicInteger idCounter;//for generating uniqeId
    private ReadWriteLock locker;
    private static class singelton{private static ConnectionImpl connections=new ConnectionImpl<>();}//we need a static action in order to be a thred safe singelton

    //singelton optional

    //constructor
    public ConnectionImpl(){
        activeClients=new HashMap<>();
        this.idCounter=new AtomicInteger(0);
        locker=new ReentrantReadWriteLock();//this lock helps with syncronisation while changing critical things
    }

    //singelton instance
    public static ConnectionImpl getInstance(){
        return singelton.connections;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        locker.readLock().lock();
        try{
            if(activeClients.containsKey(connectionId)){//client is active
                activeClients.get(connectionId).send((msg));//send throut connection handler
                return true;
            }
            else return false;
        }finally{locker.readLock().unlock();}//so it will happend after return!
    }

    @Override
    public void send(String channel, T msg) {
        String message=(String)msg;
        if(DataBase.getInstance().getGamesList(channel)!=null) {//this game exists
            for (Integer userId : DataBase.getInstance().getGamesList(channel)) {//al the users that subscribed to this game
                locker.readLock().lock();//so anyone who is subscribed to this game cant read before we write this new message
                if (activeClients.containsKey(userId)) {
                    //if a contaxt switch happend and this user unsubscribed from this chanel
                    message=message.substring(0, 21)+""+DataBase.getInstance().getSubIdConnectionId(userId,channel)+""+message.substring(21);
                    ConnectionHandler conn=activeClients.get(userId);
                    conn.send((T)message);//we now that it will be string so this casting is ok
                }
                locker.readLock().unlock();
            }
        }
        
    }
    public ReadWriteLock getLocker(){
        return locker;
    }
    public Map<Integer,ConnectionHandler<T>> getActive(){
        return activeClients;
    } 

    @Override
    public void disconnect(int connectionId) {
        locker.writeLock().lock();
        activeClients.remove(connectionId);
        locker.writeLock().unlock();
        
    }

    public int NextUniqeId(){//for getting a new id for next client
        return idCounter.incrementAndGet();
    }

    public void addNewUser(ConnectionHandler<T> newConnectionHandler, int connectionId){//when a first connection established
        locker.writeLock().lock();               //locking writing
        activeClients.putIfAbsent(connectionId,newConnectionHandler);
        locker.writeLock().unlock();             //unlocking writing
    }
 
}
