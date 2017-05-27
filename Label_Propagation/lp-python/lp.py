#####################################
# Author:zhaozhiyong
# Date:20151205
# Fun:Label Propagation
#####################################
import string

def loadData(filePath):
    f = open(filePath)
    vector_dict = {}
    edge_dict = {}
    for line in f.readlines():
        lines = line.strip().split("\t")
	
        for i in xrange(2):
            if lines[i] not in vector_dict:
                #put the vector into the vector_dict
                vector_dict[lines[i]] = string.atoi(lines[i])
                #put the edges into the edge_dict
                edge_list = []
                if len(lines) == 3:
                    edge_list.append(lines[1-i]+":"+lines[2])
                else:
                    edge_list.append(lines[1-i]+":"+"1")
                edge_dict[lines[i]] = edge_list
            else:
                edge_list = edge_dict[lines[i]]
                if len(lines) == 3:
                    edge_list.append(lines[1-i]+":"+lines[2])
                else:
                    edge_list.append(lines[1-i]+":"+"1")
                edge_dict[lines[i]] = edge_list
    
    return vector_dict, edge_dict

def get_max_community_label(vector_dict, adjacency_node_list):
    label_dict = {}
    # generate the label_dict
    for node in adjacency_node_list:
        node_id_weight = node.strip().split(":")
        node_id = node_id_weight[0]
        node_weight = string.atoi(node_id_weight[1])
        if vector_dict[node_id] not in label_dict:
            label_dict[vector_dict[node_id]] = node_weight
        else:
            label_dict[vector_dict[node_id]] += node_weight

    # find the max label
    sort_list = sorted(label_dict.items(), key = lambda d: d[1], reverse=True)
    max_weight = 0
    new_label = -1
    for label in label_dict.keys():
        if label_dict[label] > max_weight:
            max_weight = label_dict[label]
            new_label = label
    return new_label

def check(vector_dict, edge_dict):
    for node in vector_dict.keys():
        adjacency_node_list = edge_dict[node]
        node_label = vector_dict[node]
        label_check = {}
        for ad_node in adjacency_node_list:
            node_id_weight = ad_node.strip().split(":")
            node_id = node_id_weight[0]
            node_weight = string.atoi(node_id_weight[1])
            if vector_dict[node_id] not in label_check:
                label_check[vector_dict[node_id]] = node_weight
            else:
                label_check[vector_dict[node_id]] += node_weight
        max_weight = 0
        new_label = -1
        for label in label_check.keys():
            if label_check[label] > max_weight:
                max_weight = label_check[label]
                new_label = label
       # sort_list = sorted(label_check.items(), key = lambda d: d[1], reverse=True)

      #  if node_label == sort_list[0][0]:
        if node_label == new_label:
            continue
        else:
            return 0

    return 1    

def label_propagation(vector_dict, edge_dict):
    #initial, let every vector belongs to a community
    t = 0
    #for every node in a random order
    while True:
        if (check(vector_dict, edge_dict) == 0):
            t = t+1
            print "----------------------------------------"
            print "iteration: ", t
            for node in vector_dict.keys():
                adjacency_node_list = edge_dict[node]
                vector_dict[node] = get_max_community_label(vector_dict, adjacency_node_list)
         #   print vector_dict
        else:
            break
    print t
    return vector_dict


if __name__ == "__main__":
    vector_dict, edge_dict=loadData("../dataset/Amazon/amazon.e")
    print "original community: ", vector_dict
    vec_new = label_propagation(vector_dict, edge_dict)
    label = {}
    num = 0
    for vertex in vec_new.keys():
        if vec_new[vertex] not in label:
            label[vec_new[vertex]] = 1
            num += 1
    print "---------------------------------------------------------"
    print "the final result: "
 #   for key in vec_new.keys():
 #       print str(key) + " ---> " + str(vec_new[key])
    print num
