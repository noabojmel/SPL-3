package bgu.spl.net.impl.stomp;

public class subscribeFrame {
    private String destination=null;
    private Integer id=null;
    private String receipt_id=null;
    private int headerCount=0;
    private String[]split;
    private int connectionId;

    public subscribeFrame(String[]split, int connectionId){
        this.connectionId=connectionId;
        this.split=split;
    }

    public String procces(){
        for (String line : split) {//finding the headers
            String []thisline=line.split(":"); 
            if(thisline[0].equals("receipt")){
                this.receipt_id=thisline[1];
                headerCount++;
            }
            else if(thisline[0].equals("destination")){
                this.destination=thisline[1];
                headerCount++;
            }
            else if(thisline[0].equals("id")){
                this.id=Integer.parseInt(thisline[1]);
                headerCount++;
            }
        }
        if(headerCount!=3){
            return "ERROR\nreceipt-id:"+receipt_id+"message:one of the headers is missing\u0000"; 
        }
        else{
            boolean succes= DataBase.getInstance().joinGame(connectionId,destination,id);//register if we can' also adding to users list of games
            if(succes){
                //recite
                return "RECEIPT\nreceipt-id:"+receipt_id+"\u0000";
            }
            else{
                //error
                return "ERROR\nreceipt-id:"+receipt_id+"\nmessage:you are already subsribed to topic "+destination+"\u0000";
            }
        }
    }
}
