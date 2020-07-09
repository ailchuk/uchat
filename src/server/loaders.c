#include "server.h"

// static void free_arrays(t_ld_d *arrays) {
//     free(arrays->dialog_id);
//     free(arrays->user_id);
//     free(arrays->username);
//     free(arrays);
// }

static t_ld_d *create_arrays_dlg(t_list *lst, int len) {
    t_ld_d *arrays = malloc(sizeof(t_ld_d));
    t_dialog_users *dialogue = lst->data;
    int *dids = malloc(sizeof(int) * len);
    int *uids = malloc(sizeof(int) * len);
    char **names = malloc(sizeof(char *) * len);

    for (int i = 0; i < len; i++) {
        dialogue = lst->data;
        dids[i] = dialogue->dialog_id;
        uids[i] = dialogue->user_id;
        names[i] = dialogue->username;
        lst = lst->next;
    }

    arrays->dialog_id = dids;
    arrays->user_id = uids;
    arrays->username = names;

    return arrays;
}

static t_ld_d *create_arrays_grp(t_list *lst, int len) {
    t_ld_d *arrays = malloc(sizeof(t_ld_d));
    t_all_gr_member *group = lst->data;
    int *gids = malloc(sizeof(int) * len);
    char **names = malloc(sizeof(char *) * len);

    for (int i = 0; i < len; i++) {
        group = lst->data;
        gids[i] = group->group_id;
        names[i] = group->group_name;
        lst = lst->next;
    }

    arrays->dialog_id = gids;
    arrays->username = names;
    return arrays;
}

char *mx_load_dialogues(void *jobj, t_comm *connect) {
    json_object *j_uid = NULL;
    t_list *d_lst = NULL;
    t_ld_d *arrays = NULL;
    char *res = NULL;
    int len = 0;
    int uid = 0;

    json_object_object_get_ex(jobj, "id", &j_uid);
    if (!j_uid || json_object_get_type(j_uid) != json_type_int)
        return mx_bad_request(NULL, NULL);

    uid = json_object_get_int(j_uid);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    d_lst = mx_get_dialog_users(connect->db, uid, &len);
    if (d_lst)
        arrays = create_arrays_dlg(d_lst, len);

    // if (d_lst)
    //     mx_delete_list(d_lst);
    res = mx_json_string_load_dlg(arrays, len);
    // free_arrays(arrays);
    return res;
}

char *mx_load_groups(void *jobj, t_comm *connect) {
    json_object *j_uid = NULL;
    t_list *g_lst = NULL;
    t_ld_d *arrays = NULL;
    char *res = NULL;
    int len = 0;
    int uid = 0;

    json_object_object_get_ex(jobj, "id", &j_uid);
    if (!j_uid || json_object_get_type(j_uid) != json_type_int)
        return mx_bad_request(NULL, NULL);

    uid = json_object_get_int(j_uid);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    g_lst = mx_get_groups(connect->db, uid, &len);

    if (g_lst)
        arrays = create_arrays_grp(g_lst, len);
    // if (d_lst)
    //     mx_delete_list(d_lst);
    res = mx_json_string_load_grp(arrays, len);
    // free_arrays(arrays);
    return res;
}
