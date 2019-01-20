package hu.marci.customsolutions.sudokusolveranddigitizer.models;

public interface Classifier {
    String name();

    Classification recognize(final float[] pixels);
}
