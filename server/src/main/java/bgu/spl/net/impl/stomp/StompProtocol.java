package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionImpl;
import bgu.spl.net.srv.Connections;

public class StompProtocol implements StompMessagingProtocol<String> {

    private int connectionId=-1;
    private ConnectionImpl<String> connection;
    private boolean shouldTerminate = false;

    public StompProtocol(){}

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connection = (ConnectionImpl<String>) connections;
        
    }

    @Override
    public void process(String message) {
        System.out.println(message);
        String[] split=message.split("\n");
        if(split[0].equals("SUBSCRIBE")){
            subscribeFrame ans=new subscribeFrame(split, connectionId);
            String msg=ans.procces();
            connection.send(connectionId, msg); 
            if(msg.substring(0, 1).equals("E")){//if an error came back we need to close the connection
                DataBase.getInstance().logout(connectionId);//part off closing the connection
                connection.disconnect(connectionId);//error so we need to disconnect
                shouldTerminate=true;//after all the procces id done we need to close the connection
            }
        }
        else if(split[0].equals("UNSUBSCRIBE")){
            unsubscrubeFrame ans=new unsubscrubeFrame(split, connectionId);
            String msg=ans.procces();
            connection.send(connectionId, msg); 
            if(msg.substring(0, 1).equals("E")){//if an error came back we need to close the connection
                DataBase.getInstance().logout(connectionId);//part off closing the connection
                connection.disconnect(connectionId);//error so we need to disconnect
                shouldTerminate=true;//after all the procces id done we need to close the connection
            }
        }
        else if(split[0].equals("CONNECT")){
            connectFrame ans=new connectFrame(split, connectionId,connection);
            String msg=ans.procces();
            connection.send(connectionId, msg);
            if(msg.substring(0, 1).equals("E")){//if an error came back we need to close the connection
                DataBase.getInstance().logout(connectionId);
                connection.disconnect(connectionId);//error so we need to disconnect
                shouldTerminate=true;//after all the procces id done we need to close the connection
            }
        }
        else if(split[0].equals("DISCONNECT")){
            discconectFrame ans=new discconectFrame(split, connectionId);
            String msg=ans.procces();
            connection.send(connectionId, msg);
            connection.disconnect(connectionId);
            shouldTerminate=true;
        }
        else if(split[0].equals("SEND")){
            String msg;
            String receiptId=null;
            boolean rec=false;
            String des=null;
            int len=6;
            for(int i=1;i<3;i++){//finding the headers
                if(split[i].split(":")[0].equals("destination")){
                    des=(split[i].split(":"))[1];
                    len=len+split[i].length();
                }
                if(split[i].split(":")[0].equals("receipt-id")){
                    receiptId=split[i].split(":")[2];
                    rec=true;
                    len=len+split[i].length();
                }
            }
            if(des==null){
                if(rec){msg="ERROR\nreceipt-id:"+receiptId+"\nmessage:one of the headers is missing\u0000";}
                else{msg="ERROR\nmessage:one of the headers is missing\u0000";}
                connection.send(connectionId, msg);
                DataBase.getInstance().logout(connectionId);//part off closing the connection
                connection.disconnect(connectionId);
                shouldTerminate=true;
            }
            else{//sending the messege to connection with the rigth game name, we also send a reciept if nedded
                message=message.substring(len);
                sendFrame ans=new sendFrame(message,DataBase.getInstance().getUniqeMsgId(),des);
                msg=ans.procces();
                connection.send(des, msg);
                if(receiptId!=null){
                    connection.send(connectionId,"RECEIPT\nreceipt-id:"+receiptId+"\u0000");
                }
            }
        }
        else{
            String msg="ERROR\nreceipt-id:\nmessage:server is not familiar with action "+split[0]+"\u0000";//check for reciept?
            connection.send(connectionId, msg);
            DataBase.getInstance().logout(connectionId);//part off closing the connection
            shouldTerminate=true;
        }
        
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public void terminate(){
        shouldTerminate=true;
    }
    
}
