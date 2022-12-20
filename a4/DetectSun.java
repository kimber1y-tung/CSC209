public class DetectSun {

    static final int N = 20;
    public static void main(String[] args) {

        List<List<Integer>> graph = makeGraph(N);


        System.out.println(isSun(graph));
    }


    public static boolean isSun(List<List<Integer>> graph) {

        int beginNode = 0;
        int curNode = 0;

        //find first vertex whose degree is 3
        for(int i = 0; i < graph.size(); i++) {
            if(graph.get(i).size() == 3) {
                beginNode = i;
                curNode = i;
                break;
            }
        }

        int count_single = 0;
        int count_cycle = 0;

        boolean[] visited_single = new boolean[2 * N];
        boolean[] visited_cycle = new boolean[2 * N];

        while(true) {

            if(graph.get(curNode).size() != 3) {
                return false;
            }

            int degree_one_count = 1;

            for(int adj : graph.get(curNode)) {

                if(graph.get(adj).size() != 1 && graph.get(adj).size() != 3) {
                    return false;
                }

                if(graph.get(adj).size() == 1) {
                    if(/*visited_single[adj] ||*/degree_one_count == 0) {
                        return false;
                    }else {
                        visited_single[adj] = true;
                        count_single++;
                    }
                    degree_one_count--;
                }
            }

            int degree_three_count_visited = 1;

            for(int adj : graph.get(curNode)) {

                if(graph.get(adj).size() != 1) {
                    if(beginNode == curNode) {
                        visited_cycle[curNode] = true;
                        count_cycle++;
                        curNode = adj;
                        break;
                    }else{
                        if(visited_cycle[adj]) {
                            if(degree_three_count_visited == 0) {
                                return count_single == N && count_cycle == N - 1;
                            }else {
                                degree_three_count_visited --;
                                continue;
                            }

                        }else {
                            visited_cycle[curNode] = true;
                            count_cycle++;
                            curNode = adj;
                            break;
                        }
                    }
                }
            }
        }

    }
    public static List<List<Integer>> makeGraph(int N){

        List<List<Integer>> graph = new ArrayList<>();
        for(int i = 0; i < 2 * N; i++) {
            graph.add(new ArrayList<>());
        }

        for(int i = 1; i < N - 1; i++) {
            graph.get(i).add(i + 1);
            graph.get(i).add(i - 1);
            graph.get(i).add(i + N);
        }

        graph.get(0).add(1);
        graph.get(0).add(N-1);
        graph.get(0).add(N);

        graph.get(N-1).add(0);
        graph.get(N-1).add(N-2);
        graph.get(N-1).add(2 * N - 1);

        for(int i = N; i < 2 * N; i++) {
            graph.get(i).add(i - N);
        }


        return graph;

    }

}