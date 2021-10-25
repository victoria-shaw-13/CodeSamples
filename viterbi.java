   
/*
method using the Viterbi Algo taking in two hashmaps, one for observations on transitions and one for observations on emissions and a sentence. 
*/
 public static void Viterbi(HashMap<String, HashMap<String, Double>> tMap, HashMap<String, HashMap<String, Double>> hmap, String sentence) {
        bTrack = "";
        String[] pieces = sentence.toLowerCase().split(" ");
        ArrayList<String> currStates = new ArrayList<>();
        currStates.add("#");
        HashMap<String, Double> currScores = new HashMap<>();
        currScores.put("#", 0.0); //start node
        ArrayList<HashMap<String, String>> prevMap = new ArrayList<>();

        for (int i = 0; i < pieces.length; i++) {

            ArrayList<String> nextStates = new ArrayList<>();
            HashMap<String, Double> nextScores = new HashMap<>();
            for (String currState : currStates) {
                if (tMap.containsKey(currState)) {  
                    for (String nextState : tMap.get(currState).keySet()) {
                        if (!nextStates.contains(nextState)) {
                            nextStates.add(nextState);
                        }
                        Double observationScore;
                        Double transitionScore;
                        if (hmap.get(nextState) != null) {
                            if (hmap.get(nextState).containsKey(pieces[i])) {
                                observationScore = hmap.get(nextState).get(pieces[i]);
                            } else {
                                observationScore = unseenPenalty;
                            }
                        } else {
                            int count = 1 / 0;
                            observationScore = unseenPenalty;
                        }

                        if (tMap.get(currState).get(nextState) != null) {
                            transitionScore = tMap.get(currState).get(nextState);
                        } else {
                            int count = 1 / 0;
                            transitionScore = unseenPenalty;
                        }


                        Double nextScore = currScores.get(currState) + transitionScore + observationScore;

                        //System.out.println(pieces[i]+"current state:"+ currState+ "next state:"+nextState + nextScore);

                        //puts override, so if nextScore is larger , nextState gets replace in prevMap and NextScores
                        if (!nextScores.containsKey(nextState) || nextScore > nextScores.get(nextState)) {
                            nextScores.put(nextState, nextScore);
                            //putting the new next score into the prev map
                            if (prevMap.size() <= i) {
                                HashMap<String, String> inMap = new HashMap<>();
                                inMap.put(nextState, currState);
                                prevMap.add(inMap);
                            } else {
                                prevMap.get(i).put(nextState, currState);

                            }
                        }

                    }

                }

            }

            currStates = nextStates;
            currScores = nextScores;


        }


        Stack<String> backTrack = new Stack<>();

        //picks the best score word to start off at
        String cState=null;
        for(String bestscore: prevMap.get(pieces.length-1).keySet())
        {
            if(cState==null)
            {
                cState=bestscore;
            }
            if(currScores.get(bestscore)>currScores.get(cState))
            {
                cState=bestscore;
            }

        }
        //System.out.println(cState);

        for (int k = pieces.length-1; k >= 0; k--) {
            backTrack.add(0, cState);
            cState = prevMap.get(k).get(cState);
        }

        System.out.println(backTrack.toString());
        for (int h = 0; h < backTrack.size(); h++){
            String s = backTrack.get(h);
            bTrack += s + " ";
            //store tags in a string so they can be compared to given tags file find # correct/incorrect
        }


    }