package bgu.spl.net.impl.stomp;

public class unsubscrubeFrame {
    private String destination=null;
    private Integer id=null;
    private String receipt_id=null;
    private int headerCount=0;
    private String[]split;
    private int connectionId;

    public unsubscrubeFrame(String[]split, int connectionId){
        this.connectionId=connectionId;
        this.split=split;
    }

    public String procces(){
        for (String line : split) {//finding the headers
            String []thisline=line.split(":"); 
            if(thisline[0].equals("receipt")){
                if(thisline.length>1){this.receipt_id=thisline[1];
                headerCount++;}
            }
            else if(thisline[0].equals("id")){
                if(thisline.length>1){
                this.id=Integer.parseInt(thisline[1]);
                headerCount++;
            }
            }
        }
        if(headerCount!=2){
            return "ERROR\nreceipt-id:"+receipt_id+"\nmessage:one of the headers is missing\u0000"; 
        }
        else{
            boolean succes= DataBase.getInstance().exitGame(connectionId,id);
            if(succes){
                //recite
                return "RECEIPT\nreceipt-id:"+receipt_id+"\u0000";
            }
            else{
                //error
                return "ERROR\nreceipt-id:"+receipt_id+"\nmessage:you were not subscribed to the topic you requsted, cannot unsuscribe\u0000";
            }
        }
    }   
}
