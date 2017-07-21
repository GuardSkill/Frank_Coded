using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.NetworkInformation;
using System.DirectoryServices;
using System.Security.Principal;
using System.DirectoryServices.AccountManagement;

namespace ADGroup
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
               //PrincipalContext context = new PrincipalContext(ContextType.Domain, System.Net.NetworkInformation.IPGlobalProperties.GetIPGlobalProperties().DomainName);  
                PrincipalContext context = new PrincipalContext(ContextType.Domain, System.Net.NetworkInformation.IPGlobalProperties.GetIPGlobalProperties().DomainName);      //get current domain context 
                ComputerPrincipal computerPrincipal = ComputerPrincipal.FindByIdentity(context,Environment.MachineName);
                string computerSid = computerPrincipal.Sid.Value;                                                                 //Get current pc sid in domain
                string[] computerSidParts = computerSid.Split('-');
                int i = computerSid.Length;
                while (!computerSid[i - 1].Equals('-'))
                {
                    i--;
                }
                string DomainUsersSid = computerSid.Substring(0, i) + "513";                                                        //get Domain Users Sid
                Console.WriteLine(DomainUsersSid);
                DirectoryEntry directory_entry = new DirectoryEntry(string.Format("LDAP://{0}", "jhinno.com"));
                DirectorySearcher searcher = new DirectorySearcher(directory_entry)
                {
                    Filter = string.Format("(&(objectCategory=group)(objectClass=group)(objectSid={0}))", DomainUsersSid)  //primaryGroupID=513  (primaryGroupToken=513)  S-1-5-21-domain-513   objectSid=*-513 name=Domain Users S-1-5-21-*-*-*-513
                };                              //objectSid=S-1-5-21-1476668093-2663664621-3836243857-513          only this is valid 
                SearchResult result = searcher.FindOne();
                DirectoryEntry entry = result.GetDirectoryEntry();
                string shortname = entry.Name.Substring(3, entry.Name.Length - 3);   //in the front of the entry.Name is CN=
                Console.WriteLine(shortname);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
