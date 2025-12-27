//Suppose writer is given priority over reader i.e writer can write only when no reader is reading, 
//but reader can read only when no writer is writing and no writer is waiting for writing.
//Multiple readers can read at the same time.
//Multiple writers cannot write at the same time.

import java.io.*;
class Student implements Serializable
{
    private int roll;
    private String name;
    private float cgpa;
    public Student(int roll, String name, float cgpa)
    {
        this.roll = roll;
        this.name = name;   
        this.cgpa=cgpa;
    }
    public String toString()
    {
        return "Roll= "+roll+"Name= "+name+"CGPA= "+cgpa;
    }
}
class StudentFile
{
    public void write(Student s)throws IOException
    {
        try{
        FileOutputStream fout=new FileOutputStream("Report Card.dat",true);
        ObjectOutputStream oout=new ObjectOutputStream(fout);
        oout.writeObject(s);
        oout.close();
        }
        catch(IOException e){System.exit(1);}
    }
    public void read()
    {
        try{
        FileInputStream fin=new FileInputStream("Report Card.dat");
        ObjectInputStream oin=new ObjectInputStream(fin);
        try{
            while(true)
            {
                Student s=(Student)oin.readObject();
                System.out.println(s);
            }
        }
        catch(EOFException e){}
        catch(ClassNotFoundException e){System.exit(1);}             
        finally{
            oin.close();
        }
        }
        catch(IOException e){System.exit(1);}   
    }
}
class Lock
{
    int rcnt;
    int wcnt;
    int waitwcnt;
    public Lock()
    {
        this.rcnt=0;
        this.wcnt=0;
        this.waitwcnt=0;
    }
    synchronized public void readLock()
    {
        while(wcnt>0 || waitwcnt>0)
        {
            try{
                wait();
            }
            catch(InterruptedException e){}
        }
        rcnt++;
    }
    synchronized public void readUnlock()
    {
        rcnt--;
        notifyAll();
    }
    synchronized public void writeLock()
    {
        waitwcnt++;
        while(rcnt>0 || wcnt>0)
        {
            try{
                wait();
            }
            catch(InterruptedException e){}
        }
        waitwcnt--;
        wcnt++;
    }
    synchronized public void writeUnlock()
    {
        wcnt--;
        notifyAll();
    }
}
class Reader implements Runnable
{
    private StudentFile sf;
    private Lock lock;
    private Thread t;
    public Reader(StudentFile sf,Lock lock)
    {
        this.sf=sf;
        this.lock=lock;
        t=new Thread(this);
    }
    public void run()
    {
        lock.readLock();
        sf.read();
        lock.readUnlock();
    }
    public void start()
    {
        t.start();
    }
}
class Writer implements Runnable
{
    private StudentFile sf;
    private Lock lock;
    private Thread t;
    public Writer(StudentFile sf,Lock lock)
    {
        this.sf=sf;
        this.lock=lock;
        t=new Thread(this);
    }
    public Student accept()throws IOException
    {
        int roll;String name;float cgpa;
        BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Enter roll no.");
        roll=Integer.parseInt(br.readLine());
        System.out.println("Enter name");
        name=br.readLine();
        System.out.println("Enter cgpa");
        cgpa=Float.parseFloat(br.readLine());
        Student s=new Student(roll,name,cgpa);
        return s;
    }
    public void run()
    {
        try{
        lock.writeLock();
        sf.write(accept());
        lock.writeUnlock();
        }
        catch(IOException e){System.exit(1);}
    }
    public void start()
    {
        t.start();
    }
}
class ImplementReaderWriter
{
    public static void main(String[] args)
    {
        StudentFile sf=new StudentFile();
        Lock lock=new Lock();
        Writer w=new Writer(sf,lock);
        Reader r=new Reader(sf,lock);
        w.start();
        r.start();
    }
}