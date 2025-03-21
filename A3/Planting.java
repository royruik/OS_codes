// Name: Roy Rui
// Student id: 300176548
//
// The Planting Synchronization Problem
//
import java.util.Random;
import java.util.concurrent.Semaphore;

public class Planting {
    public static void main(String args[]) {
        // Create semaphores
        Semaphore shovel = new Semaphore(1); // Shared shovel between Student and TA
        Semaphore emptyHoles = new Semaphore(0); // Signals an empty hole is available
        Semaphore unfilledHoles = new Semaphore(0); // Signals a planted hole to be filled
        Semaphore maxLimit = new Semaphore(5); // Limit unfilled holes to MAX=5

        // Create TA, Professor, and Student threads
        TA ta = new TA(shovel, emptyHoles, unfilledHoles, maxLimit);
        Professor prof = new Professor(emptyHoles, unfilledHoles, ta);
        Student stdnt = new Student(shovel, emptyHoles, maxLimit, ta);

        // Start the threads
        prof.start();
        ta.start();
        stdnt.start();

        // Wait for Professor to finish
        try { prof.join(); } catch (InterruptedException e) { }

        // Stop TA and Student
        ta.interrupt();
        stdnt.interrupt();
    }
}

class Student extends Thread {
    private Semaphore shovel, emptyHoles, maxLimit;
    private TA ta;

    public Student(Semaphore shovel, Semaphore emptyHoles, Semaphore maxLimit, TA ta) {
        this.shovel = shovel;
        this.emptyHoles = emptyHoles;
        this.maxLimit = maxLimit;
        this.ta = ta;
    }

    public void run() {
        while (true) {
            try {
                if (ta.getHoleDug() - ta.getHoleFilled() >= ta.getMAX()) {
                    System.out.println("Student: Must wait for TA " + ta.getMAX() + " holes ahead");
                    Thread.sleep(50);
                    continue;
                }
                maxLimit.acquire();
                shovel.acquire();
                System.out.println("Student: Got the shovel");
                Thread.sleep((int) (100 * Math.random()));
                ta.incrHoleDug();
                System.out.println("Student: Hole " + ta.getHoleDug() + " Dug");
                emptyHoles.release();
                System.out.println("Student: Letting go of the shovel");
                shovel.release();
            } catch (InterruptedException e) {
                break;
            }
        }
        System.out.println("Student is done");
    }
}

class TA extends Thread {
    private Semaphore shovel, emptyHoles, unfilledHoles, maxLimit;
    private int holeFilledNum = 0;
    private int holePlantedNum = 0;
    private int holeDugNum = 0;
    private final int MAX = 5;
    private Semaphore mutex = new Semaphore(1); // Replaces synchronized

    public int getMAX() { return MAX; }

    public void incrHoleDug() {
        try {
            mutex.acquire();
            holeDugNum++;
        } catch (InterruptedException e) {
        } finally {
            mutex.release();
        }
    }

    public void incrHolePlanted() {
        try {
            mutex.acquire();
            holePlantedNum++;
        } catch (InterruptedException e) {
        } finally {
            mutex.release();
        }
    }

    public int getHoleDug() {
        try {
            mutex.acquire();
            return holeDugNum;
        } catch (InterruptedException e) {
            return -1;
        } finally {
            mutex.release();
        }
    }

    public int getHolePlanted() {
        try {
            mutex.acquire();
            return holePlantedNum;
        } catch (InterruptedException e) {
            return -1;
        } finally {
            mutex.release();
        }
    }

    public int getHoleFilled() {
        try {
            mutex.acquire();
            return holeFilledNum;
        } catch (InterruptedException e) {
            return -1;
        } finally {
            mutex.release();
        }
    }

    public void incrHoleFilled() {
        try {
            mutex.acquire();
            holeFilledNum++;
        } catch (InterruptedException e) {
        } finally {
            mutex.release();
        }
    }

    public TA(Semaphore shovel, Semaphore emptyHoles, Semaphore unfilledHoles, Semaphore maxLimit) {
        this.shovel = shovel;
        this.emptyHoles = emptyHoles;
        this.unfilledHoles = unfilledHoles;
        this.maxLimit = maxLimit;
    }

    public void run() {
        while (true) {
            try {
                unfilledHoles.acquire();
                shovel.acquire();
                System.out.println("TA: Got the shovel");
                Thread.sleep((int) (100 * Math.random()));
                incrHoleFilled();
                System.out.println("TA: The hole " + getHoleFilled() + " has been filled");
                System.out.println("TA: Letting go of the shovel");
                shovel.release();
                maxLimit.release();
            } catch (InterruptedException e) {
                break;
            }
        }
        System.out.println("TA is done");
    }
}

class Professor extends Thread {
    private Semaphore emptyHoles, unfilledHoles;
    private TA ta;

    public Professor(Semaphore emptyHoles, Semaphore unfilledHoles, TA ta) {
        this.emptyHoles = emptyHoles;
        this.unfilledHoles = unfilledHoles;
        this.ta = ta;
    }

    public void run() {
        while (ta.getHolePlanted() < 21) {
            try {
                emptyHoles.acquire();
                Thread.sleep((int) (50 * Math.random()));
                ta.incrHolePlanted();
                System.out.println("Professor: All be advised that I have completed planting hole " + ta.getHolePlanted());
                unfilledHoles.release();
            } catch (InterruptedException e) {
                break;
            }
        }
        System.out.println("Professor: We have worked enough for today");
    }
}
