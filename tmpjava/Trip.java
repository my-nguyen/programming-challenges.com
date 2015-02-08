import java.util.ArrayList;
import java.util.Collections;


class Trip {
    public ArrayList<Float> _expenses = new ArrayList<Float>();


    public void sort() {
        Collections.sort(_expenses);
    }


    public String toString() {
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < _expenses.size(); i++) {
            builder.append(_expenses.get(i));
            builder.append(" ");
        }
        builder.append("\n");

        return builder.toString();
    }


    private float average() {
        float total = 0.0f;
        for (int i = 0; i < _expenses.size(); i++)
            total += _expenses.get(i);

        if (Debug._level >= Debug.COPIOUS)
            System.out.println("AVERAGE: " + (total / _expenses.size()));
        return total / _expenses.size();
    }


    public float exchange() {
        float total = 0.0f;
        float avg = average();
        for (int i = 0; i < _expenses.size() && _expenses.get(i) < avg; i++)
            total += avg - _expenses.get(i);
        return total;
    }
}
