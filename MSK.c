#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

// Fonction pour convertir une chaine en adresse IP 32 bits
uint32_t ip_to_uint32(const char *ip_str) {
    struct in_addr addr;
    inet_pton(AF_INET, ip_str, &addr);
    return ntohl(addr.s_addr);
}

// Fonction pour calculer l'adresse reseau
uint32_t calculate_network_address(uint32_t ip, uint32_t mask) {
    return ip & mask;
}

// Fonction pour calculer l'adresse de diffusion
uint32_t calculate_broadcast_address(uint32_t network, uint32_t mask) {
    return network | ~mask;
}

// Fonction pour calculer le nombre d'hôtes
uint32_t calculate_number_of_hosts(uint32_t mask) {
    int num_zero_bits = 32;
    while (mask > 0) {
        mask <<= 1;
        num_zero_bits--;
    }
    return (1 << num_zero_bits) - 2;
}

int main() {
    char *data;
    char ip_str[16], mask_str[16];
    uint32_t ip, mask;
    
    // Envoyer l'en-tete HTTP
    printf("Content-Type: text/html\n\n");
    
    // Lire les donnees POST
    data = getenv("QUERY_STRING");
    if (data == NULL) {
        printf("<h1>Erreur</h1><p>Aucune donnee recue.</p>");
        return 1;
    }
    
    // Extraire les valeurs IP et masque de la chaine de requete
    sscanf(data, "ip=%15[^&]&mask=%15s", ip_str, mask_str);
    
    // Convertir les chaines en valeurs 32 bits
    ip = ip_to_uint32(ip_str);
    mask = ip_to_uint32(mask_str);
    
    // Calculer l'adresse reseau, l'adresse de diffusion et le nombre d'hotes
    uint32_t network = calculate_network_address(ip, mask);
    uint32_t broadcast = calculate_broadcast_address(network, mask);
    uint32_t num_hosts = calculate_number_of_hosts(mask);
    
    // Afficher les resultats
    printf("<html>");
    printf("<body>");
    printf("<center><h1>Resultats</h1></center>");
    printf("<p><strong>Adresse IP :</strong> %s</p>", ip_str);
    printf("<p><strong>Masque de sous-reseau :</strong> %s</p>", mask_str);
    printf("<p><strong>Adresse reseau :</strong> %s</p>", inet_ntoa((struct in_addr){htonl(network)}));
    printf("<p><strong>Adresse de diffusion :</strong> %s</p>", inet_ntoa((struct in_addr){htonl(broadcast)}));
    printf("<p><strong>Nombre hotes utilisables :</strong> %u</p>", num_hosts);
    printf("</body>");
    printf("</html>");
    return 0;
}
